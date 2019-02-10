#ifndef BINARY_H
#define BINARY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef long Bin;

Bin bin_create(const char *pstr, unsigned int isize);
void bin_free(Bin pbin);

//void bin_copy(Bin *pBTo, Bin *pBFrom);
//bool bin_init(Bin bin);
//void bin_lshift(Bin bin, int iShift);
//void bin_rshift(Bin bin, int iShift);
void bin2bstr(Bin bin, char *szStr, size_t iLen);
//bool bin_eq(Bin *pB1, Bin *pB2);
//bool bin_init_istr(Bin bin, char *szInt, size_t iLen);

Bin bin_add(Bin a, Bin b);
Bin bin_sub(Bin a, Bin b);
Bin bin_mul(Bin a, Bin b);
Bin bin_div(Bin a, Bin b);
//void bin_add(Bin *pB1, Bin *pB2, Bin *pBRet);
//void bin_sub(Bin *pB1, Bin *pB2, Bin *pBRet);
//void bin_mul(Bin *pB1, Bin *pB2, Bin *pBRet);
//void bin_div(Bin *pB1, Bin *pB2, Bin *pBRet);

#endif
