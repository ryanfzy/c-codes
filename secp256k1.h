#ifndef SECP256K1_H
#define SECP256K1_H

#include <stdlib.h>
#include <stdio.h>
#include "binary.h"

void sign(Bin d, Bin h, Bin *ps, Bin *pr);
bool verify(Bin x, Bin y, Bin r, Bin s, Bin h);

#endif
