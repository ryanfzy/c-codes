#include "rc4.h"

static void _debug_print_hex(unsigned char *str, size_t len)
{
    for (int i = 0; i < len; i++)
        printf("%02x ", str[i]);
    printf("\n");
}

static void _swap(unsigned char *i, unsigned char *j)
{
    size_t t = *i;
    *i = *j;
    *j = t;
}

static void _init_s(unsigned char *S, unsigned char *key, size_t key_len)
{
    for (size_t i = 0; i < 256; i++)
        S[i] = i;
    for (size_t i = 0, j = 0; i < 256; i++)
    {
        j = (j + S[i] + key[i % key_len]) % 256;
        _swap(&S[i], &S[j]);
    }
}

static void _encrypt(unsigned char *S, unsigned char *text, size_t text_len, unsigned char *cipher, size_t cipher_len)
{
    size_t i = 0, j = 0;
    for (size_t t = 0; t < text_len; t++)
    {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        _swap(&S[i], &S[j]);
        unsigned char k = S[(S[i] + S[j]) % 256];
        cipher[t] = text[t] ^ k;
    }
}

void rc4(unsigned char *text, size_t text_len, unsigned char *key, size_t key_len, unsigned char *cipher, size_t cipher_len)
{
    unsigned char S[255] = {'\0'};
    _init_s(S, key, key_len);
    _encrypt(S, text, text_len, cipher, cipher_len);
}