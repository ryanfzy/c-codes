#ifndef NUMBER_H
#define NUMBER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void int2str(int iNum, int iBase, char *szStr, size_t iLen);
int str2int(char *szStr, size_t iLen, int iBase);

typedef struct _binary
{
    char cBin[32];
    size_t iLen;
} Bin;

void int2bin(Bin *pBin, int iNum);
void bin_add(Bin *pB1, Bin *pB2, Bin *pBRet);
void bin_mul(Bin *pB1, Bin *pB2, Bin *pBRet);
void bin_sub(Bin *pB1, Bin *pB2, Bin *pBRet);
void bin_div(Bin *pB1, Bin *pB2, Bin *pBRet);
bool bin_eq(Bin *pB1, Bin *pB2);

void bin_add_ex(char *szB1, size_t iLen1, char *szB2, size_t iLen2, char *szRet, size_t iLenRet);
void bin_mul_ex(char *szB1, size_t iLen1, char *szB2, size_t iLen2, char *szRet, size_t iLenRet);
void bin_sub_ex(char *szB1, size_t iLen1, char *szB2, size_t iLen2, char *szRet, size_t iLenRet);
void bin_div_ex(char *szB1, size_t iLen1, char *szB2, size_t iLen2, char *szRet, size_t iLenRet);

#endif
