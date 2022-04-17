#ifndef RC4_H
#define RC4_H

#include <stdio.h>
#include <stdlib.h>

void rc4(unsigned char *text, size_t text_len, unsigned char *key, size_t key_len, unsigned char *cipher, size_t cipher_len);

#endif