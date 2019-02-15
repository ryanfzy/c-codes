#ifndef SHA256_H
#define SHA256_H

#include <stdlib.h>
#include <stdio.h>

typedef long Hash;

Hash sha_create(char *pinput, unsigned int length);
void sha_free(Hash hash);
void sha2xstr(Hash hash, char *str, size_t len);

#endif
