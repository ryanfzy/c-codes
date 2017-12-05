#ifndef DICT_H
#define DICT_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "list.h"

typedef struct _dict
{
    SList nodes[256];
} Dict;

Dict* dict_create();
void dict_init(Dict *pdict);
void dict_free(Dict *pdict);
void dict_destroy(Dict *pdict);

void dict_add(Dict *pdict, char *pkey, size_t iksize, char *pvalue, size_t ivalsize);
void dict_set(Dict *pdict, char *pkey, size_t iksize, char *pvalue, size_t ivalsize);
char* dict_get(Dict *pdict, char *pkey, size_t iksize);
bool dict_contains(Dict *pdict, char *pkey, size_t iksize);
unsigned int dict_get_count(Dict *pdict);
//void dict_remove(Dict *pdict, char *pkey);

#endif
