#ifndef CALCULATION_H
#define CALCULATION_H

#include <wchar.h>
#include "lib/stack.h"

typedef enum
{
    CALITEMTYPE_NUM,
    CALITEMTYPE_PLUS,
    CALITEMTYPE_MINUS,
    CALITEMTYPE_DIVIDE,
    CALITEMTYPE_MULTIPLY,
} CalItemType;

typedef struct tagCalItemInfo
{
    long lnum;
    CalItemType etype;
} CalItemInfo;

long calculate_result(Stack *ps, wchar_t *pret);

#endif
