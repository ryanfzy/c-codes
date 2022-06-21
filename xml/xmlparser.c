#include <ctype.h>
#include "xmlparser.h"

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
} _XmlParser;

typedef enum _ExprType
{
    EMPTY = 100,
    XML,
    EL,
    EL1,
    EL2,
    AL,
    ATTR,
    VAL,
} ExprType;

static int _expr_list[11][10] =
{
    {1, EL},
    {4, LEFT_ANGLE, IDENTIFIER, AL, EL1},
    {2, SLASH, RIGHT_ANGLE},
    {6, RIGHT_ANGLE, EL2, LEFT_ANGLE, SLASH, IDENTIFIER, RIGHT_ANGLE},
    {1, EL},
    {1, TEXT},
    {2, ATTR, AL},
    {1, EMPTY},
    {2, IDENTIFIER, VAL},
    {2, EQUAL, QUOTED_STRING},
    {1, EMPTY},
};

static int _expr_table[7][7] =
{
    {ER, 0, ER, ER, ER, ER, ER},
    {ER, 1, ER, ER, ER, ER, ER},
    {ER, ER, 3, 2,  ER, ER ,ER},
    {5,  4, ER, ER, ER, ER, ER},
    {ER, ER, 7, 7,  ER, ER, 6 },
    {ER, ER, ER, ER, ER, ER, 8},
    {ER, ER, 10, 10, ER, 9, 10},
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
    p->_count = 0;
    p->_start = 0;
    stack_init(&p->_stack);
    _PUSH(&p->_stack, EL);
}

void _xmlparser_destroy(_XmlParser *p)
{
    if (p != NULL)
    {
        stack_destroy(&p->_stack);
    }
}

XmlParser xmlparser_create()
{
    _XmlParser *p = (_XmlParser*)malloc(sizeof(_XmlParser));
    if (p != NULL)
        _xmlparser_init(p);
    return (long)p;
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

static bool _feed_char(_XmlParser *p, char ch, XmlToken *token)
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
        token->length = p->_count - p->_start - 1;
        p->_row = _token_table[0][i];
        prev_row = 0;
    }
    if (prev_row == 0 && p->_row > 0)
        p->_start = p->_count -1;
    return true;
}

static bool _feed(_XmlParser *p, char ch, XmlToken *token)
{
    p->_count++;
    if (p->_parse_tag)
    {
        bool ret = _feed_char(p, ch, token);
        if (token->type == RIGHT_ANGLE)
            p->_parse_tag = false;
        return ret;
    }
    else if (ch == '<')
    {
        p->_parse_tag = true;
        _feed_char(p, ch, token);
        if (p->_found_text)
        {
            token->type = TEXT;
            return true;
        }
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
        if (expr >= XML)
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
        {
            return _feed_token(p, token);
        }
        else
        {
            return expr == token->type;
        }
    }
    return false;
}

bool xmlparser_feed(XmlParser parser, char ch, XmlToken *token)
{
    _XmlParser *p = (_XmlParser*)parser;
    if (p != NULL && token != NULL)
    {
        if (_feed(p, ch, token))
        {
            if (token->type != NONE)
                return _feed_token(p, token);
            return true;
        }
    }
    return false;
}