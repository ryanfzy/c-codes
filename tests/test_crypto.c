#include <stdlib.h>
#include <stdio.h>
#include "crypto\rc4.h"

void _print_hex(unsigned char *str, size_t len)
{
    for (int i = 0; i < len; i++)
        printf("%02x ", str[i]);
    printf("\n");
}

int main(int argc, char *argv[])
{
    unsigned char text[45] = "The quick brown fox jumps over the lazy dog.\0";
    unsigned char cipher[44] = {'\0'};
    unsigned char text2[45] = {'\0'};
    unsigned char key[7] = "abcdefg";
    printf("text: %s\n", text);
    printf("key:");
    _print_hex(key, sizeof(key));
    rc4(text, 44, key, 7, cipher, 44);
    printf("cipher:\n");
    _print_hex(cipher, sizeof(cipher));
    rc4(cipher, 44, key, 7, text2, 44);
    printf("text2: %s\n", text2);
}