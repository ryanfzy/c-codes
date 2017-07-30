#ifndef BINARY_H
#define BINARY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BIN_LEN 32

typedef struct _binary
{
    char cBin[BIN_LEN];
} Bin;

bool bin_init_istr(Bin *pBin, char *szInt, size_t iLen);
bool bin_init_bstr(Bin *pBin, char *szBin, size_t iLen);
bool bin_init_fstr(Bin *pBin, char *szFloat, size_t iLen);

void bin2bstr(Bin *pBin, char *szBin, size_t iLen);

void bin_add(Bin *pB1, Bin *pB2, Bin *pBRet);
void bin_mul(Bin *pB1, Bin *pB2, Bin *pBRet);
void bin_sub(Bin *pB1, Bin *pB2, Bin *pBRet);
void bin_div(Bin *pB1, Bin *pB2, Bin *pBRet);
bool bin_eq(Bin *pB1, Bin *pB2);

void bin_lshift(Bin *pBin, int iShift);
void bin_rshift(Bin *pBin, int iShift);

#endif
