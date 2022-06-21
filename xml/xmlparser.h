#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "stack.h"

typedef enum _XmlTokenType
{
    ER = -1,
    NONE = 0,
    TEXT,
    LEFT_ANGLE,
    RIGHT_ANGLE,
    SLASH,
    QUOTED_STRING,
    EQUAL,
    IDENTIFIER,
} XmlTokenType;

typedef struct _XmlToken 
{
    XmlTokenType type;
    int start;
    int length;
} XmlToken;

typedef long XmlParser;

XmlParser xmlparser_create();
void xmlparser_free(XmlParser);
bool xmlparser_feed(XmlParser, char ch, XmlToken *token);

#endif