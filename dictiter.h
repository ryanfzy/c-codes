#ifndef DICTITER_H
#define DICTITER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "dict.h"

typedef struct _dictIter
{
    Dict *pDict;
    int iNode;
    int iPos;
} DictIter;

DictIter* dict_get_iter(Dict *pDict);
void dictiter_free(DictIter *pIter);

char* dictiter_get_key(DictIter *pIter);
char* dictiter_get_value(DictIter *pIter);
bool dictiter_move_next(DictIter *pIter);

#endif
