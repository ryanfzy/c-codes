#include <ctype.h>
#include "xml.h"

static int _token_table[23][8] =
{
//    alnum  <      /      >      "      space   =      non-space
    { ER,    14,    ER,    ER,    ER,    ER,     ER,    ER },  // 0 - only accept <
    { 2,     ER,    3,     ER,    ER,    ER,     ER,    ER },  // 1 : <
    { 2,     ER,    ER,    5,     ER,    6,      ER,    ER },  // 2 : start tag
    { 4,     ER,    ER,    ER,    ER,    ER,     ER,    ER },  // 3 : </
    { 4,     ER,    ER,    20,    ER,    ER,     ER,    ER },  // 4 : end tag
    { 21,    1,     ER,    ER,    21,    21,     21,    21 },  // 5 : > (OEPN_TAG)
    { 7,     ER,    12,    ER,    ER,    ER,     ER,    ER },  // 6 : starting attribute (OPEN_TAG)
    { 7,     ER,    ER,    15,    ER,    16,     8,     ER },  // 7 : attribute key
    { ER,    ER,    ER,    ER,    9,     ER,     ER,    ER },  // 8 : = (ATTR_NAME)
    { 10,    ER,    ER,    ER,    ER,    ER,     ER,    ER },  // 9 : starting attribute value
    { 10,    ER,    ER,    ER,    11,    ER,     ER,    ER },  // 10 : attribute value
    { ER,    ER,    ER,    19,    ER,    18,     ER,    ER },  // 11 : end attribute value (ATTR_VAL)
    { ER,    ER,    ER,    13,    ER,    ER,     ER,    ER },  // 12 : />
    { 13,    1,     ER,    ER,    13,    13,     13,    13 },  // 13 : text (CLOSE_TAG)
    { 2,     ER,    3,     ER,    ER,    ER,     ER,    ER },  // 14 : <
    { 21,    1,     ER,    ER,    21,    21,     21,    21 },  // 15 : text (ATTR)
    { 17,    ER,    12,    ER,    ER,    ER,     ER,    ER },  // 16 : attribute (ATTR)
    { 17,    ER,    ER,    15,    ER,    16,     8,     ER },  // 17 : attribute
    { 7,     ER,    12,    ER,    ER,    ER,     ER,    ER },  // 18 : starting attribute
    { 21,    1,     ER,    ER,    21,    21,     21,    21 },  // 19 : text
    { 21,    1,     ER,    ER,    21,    21,     21,    21 },  // 20 : text (CLOSE_TAG)
    { 21,    22,    ER,    ER,    21,    21,     21,    21 },  // 21 : text
    { 2,     ER,    3,     ER,    ER,    ER,     ER,    ER },  // 22 : < (TEXT)
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

void xmlparser_init(XmlParser *p)
{
    if (p != NULL)
    {
        p->_row = 0;
        p->_open_tag_start = 0;
        p->_open_tag_length = 0;
        p->token = NONE;
        p->start = 0;
        p->length = 0;
        p->count = 0;
    }
}

bool xmlparser_feed(XmlParser *p, char ch)
{
    if (p != NULL)
    {
        if (p->_row == ER)
        {
            p->token = ER;
            return true;
        }
        p->count++;
        int i = _char_to_index(ch);
        int prev_r = p->_row;
        p->_row = _token_table[p->_row][i];
        if (p->_row != ER)
        {
            if (prev_r != p->_row)
            {
                switch (p->_row)
                {
                    case 2:  // attribute key
                    case 4:  // close tag
                    case 7:  // open tag
                    case 10:  // attribute value
                    case 17:  // attribute key
                    case 21:  // text
                        p->start = p->count-1;
                        break;
                    case 5:
                    case 6:
                        p->token = OPEN_TAG;
                        p->length = p->count - p->start - 1;
                        p->_open_tag_start = p->start;
                        p->_open_tag_length = p->length;
                        return true;
                    case 8:
                        p->token = ATTR_NAME;
                        p->length = p->count - p->start - 1;
                        return true;
                    case 11:
                        p->token = ATTR_VAL;
                        p->length = p->count - p->start - 1;
                        return true;
                    case 15:
                    case 16:
                        p->token = ATTR;
                        p->length = p->count - p->start - 1;
                        return true;
                    case 13:
                        p->token = CLOSE_TAG;
                        p->start = p->_open_tag_start;
                        p->length = p->_open_tag_length;
                        return true;
                    case 20:
                        p->token = CLOSE_TAG;
                        p->length = p->count - p->start - 1;
                        return true;
                    case 22:
                        p->token = TEXT;
                        p->length = p->count - p->start - 1;
                        return true;
                    default:
                        break;
                }
            }
            p->token = NONE;
        }
        else
        {
            p->token = ER;
            return true;
        }
    }
    return false;
}