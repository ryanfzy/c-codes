#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef long XmlParser;

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

XmlParser xmlparser_create();
bool xmlparser_feed(XmlParser, char ch, XmlToken *token);
void xmlparser_free(XmlParser);

#endif