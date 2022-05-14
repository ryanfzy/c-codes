#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef enum _XmlToken
{
    ER = -1,
    NONE = 0,
    TEXT,
    OPEN_TAG,
    CLOSE_TAG,
    ATTR_NAME,
    ATTR_VAL,
    ATTR,
} XmlToken;

typedef struct _XmlParser
{
    int _row;
    int _open_tag_start;
    int _open_tag_length;
    XmlToken token;
    int start;
    int length;
    int count;
} XmlParser;

void xmlparser_init(XmlParser*);
bool xmlparser_feed(XmlParser *p, char ch);

#endif