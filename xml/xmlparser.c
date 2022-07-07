#include <ctype.h>
#include "xmlparser.h"
#include "stack.h"
#include "list.h"

#define _PUSH(s, e) { int _e = e; stack_push(s, (char*)&_e, sizeof(int)); }
#define _POP(s, e) stack_pop(s, (char*)e, sizeof(int))

typedef struct __XmlParser
{
    bool _parse_tag;
    bool _found_text;
    int _row;
    int _count;
    int _start;
    Stack _stack;
    List _tokens;
    on_start_tag _start_tag_fn;
    on_close_tag _close_tag_fn;
    on_attr _attr_fn;
    on_text _text_fn;
} _XmlParser;

typedef enum _ExprType
{
    EMPTY = 100,
    XML,
    EL,
    EL1,
    EL2,
    EL3,
    EL4,
    AL,
    ATTR,
    VAL,
} ExprType;

typedef enum _SemType
{
    START_TAG = 200,
    CLOSE_TAG,
    ATTR_KEY,
    ATTR_KEY_VAL,
    STEXT,
} SemType;

static int _expr_list[14][10] =
{
    {1, EL},
    {2, LEFT_ANGLE, EL1},
    {4, IDENTIFIER, START_TAG, AL, EL2},
    {2, SLASH, RIGHT_ANGLE},
    {2, RIGHT_ANGLE, EL3},
    {2, LEFT_ANGLE, EL4},
    {4, TEXT, STEXT, LEFT_ANGLE, EL4},
    {4, IDENTIFIER, START_TAG, AL, EL2},
    {5, SLASH, IDENTIFIER, CLOSE_TAG, RIGHT_ANGLE, EL3},
    {2, ATTR, AL},
    {1, EMPTY},
    {2, IDENTIFIER, VAL},
    {3, EQUAL, QUOTED_STRING, ATTR_KEY_VAL},
    {2, ATTR_KEY, EMPTY},
};

static int _expr_table[9][7] =
{
    {ER, 0, ER, ER, ER, ER, ER},
    {ER, 1, ER, ER, ER, ER, ER},
    {ER, ER, ER, ER, ER, ER, 2},
    {ER, ER, 4,  3, ER, ER, ER},
    {6,  5, ER, ER, ER, ER, ER},
    {ER, ER, ER, 8, ER, ER,  7},
    {ER, ER, 10, 10, ER, ER, 9},
    {ER, ER, ER, ER, ER, ER,11},
    {ER, ER, 13, 13, ER, 12,13},
};

static int _token_table[8][7] =
{
    { 1,  2,  3,  4,  6,  7,  0},
    {-2, -2, -2, -2, -2, -2, -2},
    {-3, -3, -3, -3, -3, -3, -3},
    {-4, -4, -4, -4, -4, -4, -4},
    { 4,  4,  4,  5,  4,  4,  4},
    {-5, -5, -5, -5, -5, -5, -5},
    {-6, -6, -6, -6, -6, -6, -6},
    {-7, -7, -7, -7, -7,  7, -7},
};

static int _char2col(char ch)
{
    if (ch == '<') return 0;
    if (ch == '>') return 1;
    if (ch == '/') return 2;
    if (ch == '"') return 3;
    if (ch == '=') return 4;
    if (isalnum(ch)) return 5;
    if (isspace(ch)) return 6;
    return -1;
}

void _xmlparser_init(_XmlParser *p)
{
    p->_row = 0;
    p->_start = 0;
    p->_found_text = false;
    stack_init(&p->_stack);
    _PUSH(&p->_stack, EL);
    list_init(&p->_tokens);
    p->_start_tag_fn = NULL;
    p->_close_tag_fn = NULL;
    p->_attr_fn = NULL;
    p->_text_fn = NULL;
}

void _xmlparser_destroy(_XmlParser *p)
{
    if (p != NULL)
    {
        stack_destroy(&p->_stack);
        list_destroy(&p->_tokens);
    }
}

static _XmlParser* _create()
{
    _XmlParser *p = (_XmlParser*)malloc(sizeof(_XmlParser));
    if (p != NULL)
        _xmlparser_init(p);
    return p;
}

XmlParser xmlparser_create()
{
    return (long)_create();
}

void xmlparser_set_listners(XmlParser parser, on_start_tag start_fn, on_attr attr_fn, on_text text_fn, on_close_tag close_fn)
{
    
    _XmlParser *p = (_XmlParser*)parser;
    if (p != NULL)
    {
        p->_start_tag_fn = start_fn;
        p->_close_tag_fn = close_fn;
        p->_attr_fn = attr_fn;
        p->_text_fn = text_fn;
    }
}

void xmlparser_free(XmlParser parser)
{
    _XmlParser *p = (_XmlParser*)parser;
    if (p != NULL)
    {
        _xmlparser_destroy(p);
        free(p);
    }
}

static bool _feed_char(_XmlParser *p, char ch, int index, XmlToken *token)
{
    int i = _char2col(ch);
    int prev_row = p->_row;
    p->_row = _token_table[p->_row][i];
    if (p->_row == ER)
    {
        token->type = ER;
        return false;
    }
    token->type = NONE;
    if (p->_row < -1)
    {
        token->type = abs(p->_row);
        token->start = p->_start;
        token->length = index - p->_start - 1;
        p->_row = _token_table[0][i];
        prev_row = 0;
    }
    if (prev_row == 0 && p->_row > 0)
        p->_start = index -1;
    return true;
}

static bool _feed(_XmlParser *p, char ch, int *index, XmlToken *token)
{
    (*index)++;
    if (p->_parse_tag)
    {
        bool ret = _feed_char(p, ch, *index, token);
        if (token->type == RIGHT_ANGLE)
        {
            p->_parse_tag = false;
            (*index)--;
            p->_row = 0;
        }
        return ret;
    }
    else if (ch == '<')
    {
        p->_parse_tag = true;
        int start = p->_start;
        _feed_char(p, ch, *index, token);
        if (p->_found_text)
        {
            p->_found_text = false;
            token->type = TEXT;
            token->start = start;
            token->length = *index - start - 1;
        }
        return true;
    }
    p->_found_text = true;
    token->type = NONE;
    return true;
}

static bool _feed_token(_XmlParser *p, XmlToken *token)
{
    int expr;
    if (_POP(&p->_stack, &expr))
    {
        if (expr >= 200)
        {
            switch (expr)
            {
                case START_TAG:
                    if (p->_start_tag_fn != NULL)
                    {
                        XmlToken *t = (XmlToken*)list_get(&p->_tokens, -1);
                        p->_start_tag_fn(t);
                        break;
                    }
                case ATTR_KEY:
                    if (p->_attr_fn != NULL)
                    {
                        XmlToken *t = (XmlToken*)list_get(&p->_tokens, -1);
                        p->_attr_fn(t, NULL);
                        break;
                    }
                case ATTR_KEY_VAL:
                    if (p->_attr_fn != NULL)
                    {
                        XmlToken *k = (XmlToken*)list_get(&p->_tokens, -3);
                        XmlToken *v = (XmlToken*)list_get(&p->_tokens, -1);
                        p->_attr_fn(k, v);
                        break;
                    }
                case STEXT:
                    if (p->_text_fn != NULL)
                    {
                        XmlToken *t = (XmlToken*)list_get(&p->_tokens, -1);
                        p->_text_fn(t);
                        break;
                    }
                case CLOSE_TAG:
                    if (p->_close_tag_fn != NULL)
                    {
                        XmlToken *t = (XmlToken*)list_get(&p->_tokens, -1);
                        p->_close_tag_fn(t);
                        break;
                    }
                default:
                    break;
            }
            return _feed_token(p, token);
        }
        else if (expr >= XML)
        {
            int next_expr = _expr_table[expr - XML][token->type - TEXT];
            if (next_expr != ER)
            {
                int *expr_tokens = _expr_list[next_expr];
                for (int i = expr_tokens[0]; i > 0; i--)
                    _PUSH(&p->_stack, expr_tokens[i]);
                return _feed_token(p, token);
            }
        }
        else if (expr == EMPTY)
            return _feed_token(p, token);
        else
            return expr == token->type;
    }
    return false;
}

bool xmlparser_feed(XmlParser parser, char ch, int *index, XmlToken *token)
{
    _XmlParser *p = (_XmlParser*)parser;
    if (p != NULL && token != NULL && index != NULL)
    {
        if (_feed(p, ch, index, token))
        {
            if (token->type != NONE)
            {
                if (_feed_token(p, token))
                    list_add(&p->_tokens, (char*)token, sizeof(XmlToken));
                else
                    return false;
            }
            return true;
        }
    }
    return false;
}