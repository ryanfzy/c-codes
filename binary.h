#ifndef BINARY_H
#define BINARY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BIN_LEN 32
#define CHAR_BITS 8
#define CHAR_NUM BIN_LEN/CHAR_BITS

typedef struct _bin32
{
    unsigned char cBin[CHAR_NUM];
} Bin;

void bin_copy(Bin *pBTo, Bin *pBFrom);
bool bin_init(Bin *pBin);
void bin_lshift(Bin *pBin, int iShift);
void bin_rshift(Bin *pBin, int iShift);
void bin2bstr(Bin *pBin, char *szStr, size_t iLen);
bool bin_eq(Bin *pB1, Bin *pB2);
bool bin_init_istr(Bin *pBin, char *szInt, size_t iLen);

void bin_add(Bin *pB1, Bin *pB2, Bin *pBRet);
void bin_sub(Bin *pB1, Bin *pB2, Bin *pBRet);
void bin_mul(Bin *pB1, Bin *pB2, Bin *pBRet);
void bin_div(Bin *pB1, Bin *pB2, Bin *pBRet);

#endif
