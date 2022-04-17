#ifndef SECP256K1TABLE_H
#define SECP256K1TABLE_H

#include <stdlib.h>
#include "binary.h"

void secp256k1table_init();
void secp256k1table_free();
Bin secp256k1table_x(int i);
Bin secp256k1table_y(int i);

#endif
