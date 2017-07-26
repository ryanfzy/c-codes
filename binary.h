#ifndef BINARY_H
#define BINARY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct _binary
{
    char cBin[32];
} Bin;

bool bin_init_istr(Bin *pBin, char *szInt, size_t iLen);
bool bin_init_bstr(Bin *pBin, char *szBin, size_t iLen);

void bin2bstr(Bin *pBin, char *szBin, size_t iLen);

void bin_add(Bin *pB1, Bin *pB2, Bin *pBRet);
void bin_mul(Bin *pB1, Bin *pB2, Bin *pBRet);
void bin_sub(Bin *pB1, Bin *pB2, Bin *pBRet);
void bin_div(Bin *pB1, Bin *pB2, Bin *pBRet);
bool bin_eq(Bin *pB1, Bin *pB2);

void int2bin_ex(char *szInt, size_t iLenInt, char *szBin, size_t iLenBin);
void bin_add_ex(char *szB1, size_t iLen1, char *szB2, size_t iLen2, char *szRet, size_t iLenRet);
void bin_mul_ex(char *szB1, size_t iLen1, char *szB2, size_t iLen2, char *szRet, size_t iLenRet);
void bin_sub_ex(char *szB1, size_t iLen1, char *szB2, size_t iLen2, char *szRet, size_t iLenRet);
void bin_div_ex(char *szB1, size_t iLen1, char *szB2, size_t iLen2, char *szRet, size_t iLenRet);

#endif
