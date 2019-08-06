#ifndef BMATH_H
#define BMATH_H

#include <stdlib.h>
#include "binary.h"

Bin bmath_gcd(Bin x, Bin y);
Bin bmath_inverse_mod(Bin a, Bin m);

void bmath_inverse_mod2(Bin a, Bin m, Bin *ret);

#endif
