#ifndef BASE64_H
#define BASE64_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool base64_encode(const char *pin, const size_t insize, char *pout, const size_t outsize);
bool base64_decode(const char *pin, const size_t insize, char *pout, const size_t outsize);

#endif //BASE64_H
