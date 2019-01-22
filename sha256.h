#ifndef SHA256_H
#define SHA256_H

#include <stdlib.h>
#include <stdio.h>

typedef struct _hash
{
    unsigned char X[32];
} Hash;

void sha256(char *pInput, unsigned int iInputLength, Hash *pHash);

#endif
