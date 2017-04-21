#ifndef DICT_H
#define DICT_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "list.h"

/*
typedef struct _dictNode
{
    SList nodes; // list of NamedNode
} DictNode;
*/

typedef struct _dict
{
    SList nodes[256];
} Dict;

Dict* dict_create();
void dict_init(Dict *pdict);
void dict_free(Dict *pdict);
void dict_destroy(Dict *pdict);

void dict_add(Dict *pdict, char *pkey, char *pvalue, size_t ivalsize);
char* dict_get(Dict *pdict, char *pkey);
bool dict_contains(Dict *pdict, char *pkey);
//void dict_remove(Dict *pdict, char *pkey);

#endif
