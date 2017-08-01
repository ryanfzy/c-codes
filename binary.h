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
} Bin32;

typedef struct _binary
{
    char cBin[BIN_LEN];
} Bin;

void bin32_copy(Bin32 *pBTo, Bin32 *pBFrom);
bool bin32_init(Bin32 *pBin);
void bin32_lshift(Bin32 *pBin, int iShift);
void bin32_rshift(Bin32 *pBin, int iShift);
void bin322bstr(Bin32 *pBin, char *szStr, size_t iLen);

void bin32_add(Bin32 *pB1, Bin32 *pB2, Bin32 *pBRet);

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
