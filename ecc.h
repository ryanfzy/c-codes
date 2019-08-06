#ifndef ECC_H
#define ECC_H

#include <stdlib.h>
#include <stdio.h>
#include "binary.h"

#define Ecc long

Ecc ecc_create_uint(unsigned int a, unsigned int b);
void ecc_free(Ecc ecc);
void ecc_add(Ecc ecc, Bin x1, Bin y1, Bin x2, Bin y2, Bin m, Bin *rx, Bin *ry);
void ecc_double(Ecc ecc, Bin x, Bin y, Bin m, Bin *rx, Bin *ry);
void ecc_mul(Ecc ecc, Bin x, Bin y, Bin n, Bin m, Bin *rx, Bin *ry);

#endif
