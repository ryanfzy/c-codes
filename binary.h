#ifndef BINARY_H
#define BINARY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef long Bin;

Bin bin_create(const char *pstr);
Bin bin_create2(const char *pstr, unsigned int isize);
void bin_free(Bin pbin);

void bin2bstr(Bin bin, char *szStr, size_t iLen);
void bin2xstr(Bin bin, char *szStr, size_t iLen);

Bin bin_add(Bin a, Bin b);
Bin bin_sub(Bin a, Bin b);
Bin bin_mul(Bin a, Bin b);
Bin bin_div(Bin a, Bin b);
Bin bin_mod(Bin a, Bin b);

void bin_add2(Bin a, Bin b, Bin *pr);
void bin_sub2(Bin a, Bin b, Bin *pr);
void bin_mul2(Bin a, Bin b, Bin *pr);
void bin_div2(Bin a, Bin b, Bin *pr);
void bin_mod2(Bin a, Bin b, Bin *pr);

#endif
