#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef long XmlParser;

XmlParser xmlparser_create();
bool xmlparser_feed(XmlParser, char ch);
void xmlparser_free(XmlParser);

#endif