#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

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

typedef void (*on_start_tag)(XmlToken*);
typedef void (*on_close_tag)(XmlToken*);
typedef void (*on_attr)(XmlToken *key, XmlToken *val);
typedef void (*on_text)(XmlToken*);

XmlParser xmlparser_create();
void xmlparser_set_listners(XmlParser, on_start_tag, on_attr, on_text, on_close_tag);
void xmlparser_free(XmlParser);
bool xmlparser_feed(XmlParser, char ch, int *index, XmlToken *token);

#endif