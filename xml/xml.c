#include <ctype.h>
#include "xml.h"

typedef struct _xmlParser
{
    int row;
} _XmlParser;

static int _token_table[21][8] =
{
//    alnum  <      /      >      "      space   =      non-space
    { ER,    14,    ER,    ER,    ER,    ER,     ER,    ER },  // 0 - only accept <
    { 2,     ER,    3,     ER,    ER,    ER,     ER,    ER },  // 1 : < (TEXT)
    { 2,     ER,    12,    5,     ER,    6,      ER,    ER },  // 2 : start tag
    { 4,     ER,    ER,    ER,    ER,    ER,     ER,    ER },  // 3 : </
    { 4,     ER,    ER,    20,    ER,    ER,     ER,    ER },  // 4 : end tag
    { 5,     1,     ER,    ER,    5,     5,      5,     5  },  // 5 : text (OEPN_TAG)
    { 7,     ER,    ER,    ER,    ER,    ER,     ER,    ER },  // 6 : starting attribute (OPEN_TAG)
    { 7,     ER,    ER,    15,    ER,    16,     8,     ER },  // 7 : attribute key
    { ER,    ER,    ER,    ER,    9,     ER,     ER,    ER },  // 8 : = (ATTR_NAME)
    { 10,    ER,    ER,    ER,    ER,    ER,     ER,    ER },  // 9 : starting attribute value
    { 10,    ER,    ER,    ER,    11,    ER,     ER,    ER },  // 10 : attribute value
    { ER,    ER,    ER,    19,    ER,    18,     ER,    ER },  // 11 : end attribute value (ATTR_VAL)
    { ER,    ER,    ER,    13,    ER,    ER,     ER,    ER },  // 12 : />
    { 13,    1,     ER,    ER,    13,    13,     13,    13 },  // 13 : text (TAG)
    { 2,     ER,    3,     ER,    ER,    ER,     ER,    ER },  // 14 : <
    { 15,    1,     ER,    ER,    15,    15,     15,    15 },  // 15 : text (ATTR)
    { 17,    ER,    ER,    ER,    ER,    ER,     ER,    ER },  // 16 : starting attribute
    { 17,    ER,    ER,    15,    ER,    16,     8,     ER },  // 17 : attribute (ATTR)
    { 7,     ER,    ER,    ER,    ER,    ER,     ER,    ER },  // 18 : starting attribute
    { 19,    1,     ER,    ER,    19,    19,     19,    19 },  // 19 : text
    { 20,    1,     ER,    ER,    20,    20,     20,    20 },  // 20 : text (CLOSE_TAG)
};

static int _char_to_index(char ch)
{
    if (isalnum(ch)) return 0;
    if (ch == '<') return 1;
    if (ch == '/') return 2;
    if (ch == '>') return 3;
    if (ch == '"') return 4;
    if (isspace(ch)) return 5;
    if (ch == '=') return 6;
    if (!isspace(ch)) return 7;
    return -1;
}

XmlParser xmlparser_create()
{
    _XmlParser *p = malloc(sizeof(_XmlParser));
    p->row = 0;
    return (XmlParser)p;
}

void xmlparser_free(XmlParser parser)
{
    _XmlParser *p = (_XmlParser*)parser;
    if (p != NULL)
        free(p);
}

bool xmlparser_feed(XmlParser parser, char ch, XmlToken *token)
{
    _XmlParser *p = (_XmlParser*)parser;
    if (p != NULL)
    {
        if (p->row == ER)
        {
            *token = ER;
            return true;
        }
        int i = _char_to_index(ch);
        int prev_r = p->row;
        p->row = _token_table[p->row][i];
        if (p->row != ER)
        {
            if (prev_r != p->row)
            {
                switch (p->row)
                {
                    case 1:
                        *token = TEXT;
                        return true;
                    case 5:
                    case 6:
                        *token = OPEN_TAG;
                        return true;
                    case 8:
                        *token = ATTR_NAME;
                        return true;
                    case 11:
                        *token = ATTR_VAL;
                        return true;
                    case 13:
                        *token = TAG;
                        return true;
                    case 15:
                    case 17:
                        *token = ATTR;
                        return true;
                    case 20:
                        *token = CLOSE_TAG;
                        return true;
                    default:
                        break;
                }
            }
            *token = NONE;
        }
        else
        {
            *token = ER;
            return true;
        }
    }
    return false;
}