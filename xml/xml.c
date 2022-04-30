#include <ctype.h>
#include "xml.h"

enum
{
    ER = -1
};

typedef struct _xmlParser
{
    int row;
} _XmlParser;

static int _token_table[12][8] =
{
//    alnum  <      /      >      "      space   =      non-space
    { ER,    1,     ER,    ER,    ER,    ER,     ER,    ER },  // 0 - only accept <
    { 2,     ER,    3,     ER,    ER,    ER,     ER,    ER },  // 1 : <
    { 2,     ER,    ER,    5,     ER,    6,      ER,    ER },  // 2 : start tag
    { 4,     ER,    ER,    ER,    ER,    ER,     ER,    ER },  // 3 : </
    { 4,     ER,    ER,    5,     ER,    ER,     ER,    ER },  // 4 : end tag
    { 5,     1,     ER,    ER,    5,     5,      5,     5  },  // 5 : text
    { 7,     ER,    ER,    ER,    ER,    ER,     ER,    ER },  // 6 : starting attribute
    { 7,     ER,    ER,    5,     ER,    6,      8,     ER },  // 7 : attribute key
    { ER,    ER,    ER,    ER,    9,     ER,     ER,    ER },  // 8 : =
    { 10,    ER,    ER,    ER,    ER,    ER,     ER,    ER },  // 9 : starting attribute value
    { 10,    ER,    ER,    ER,    11,    ER,     ER,    ER },  // 10 : attribute value
    { ER,    ER,    ER,    5,     ER,    6,      ER,    ER },  // 11 : end attribute value
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

bool xmlparser_feed(XmlParser parser, char ch)
{
    _XmlParser *p = (_XmlParser*)parser;
    if (p != NULL)
    {
        int i = _char_to_index(ch);
        int r = _token_table[p->row][i];
        if (r != ER)
        {
            p->row = r;
            return true;
        }
    }
    return false;
}