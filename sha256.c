#include <string.h>
#include "sha256.h"

#define S2(x) ((x<<30)|(x>>2))
#define S6(x) ((x<<26)|(x>>6))
#define S7(x) ((x<<25)|(x>>7))
#define S11(x) ((x<<21)|(x>>11))
#define S13(x) ((x<<19)|(x>>13))
#define S17(x) ((x<<15)|(x>>17))
#define S18(x) ((x<<14)|(x>>18))
#define S19(x) ((x<<13)|(x>>19))
#define S22(x) ((x<<10)|(x>>22))
#define S25(x) ((x<<7)|(x>>25))
#define R3(x) (x >> 3)
#define R10(x) (x >> 10)
#define CH(x,y,z) ((x&y)^(~(x)&z))
#define MAJ(x,y,z) ((x&y)^(x&z)^(y&z))
#define E0(x) (S2(x)^S13(x)^S22(x))
#define E1(x) (S6(x)^S11(x)^S25(x))
#define Q0(x) (S7(x)^S18(x)^R3(x))
#define Q1(x) (S17(x)^S19(x)^R10(x))

#define HASH_LEN 32

typedef struct
{
    unsigned char X[HASH_LEN];
} _hash;

static long K[64] = 
{
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

void sha256(char *pInput, unsigned int iInputLength, _hash *p_hash);

static void printstr(char *pCharStr, unsigned int isize)
{
    unsigned char *pUCharStr = (unsigned char*)pCharStr;
    for (int i = 0; i < isize; i++)
    {
        if (i > 0 && i % 4 == 0)
            printf(" ");
        printf("%02x", pUCharStr[i]);
    }
    printf("\n");
}

static void printlong(unsigned long lvalue)
{
    unsigned char *pLong = (unsigned char*)&lvalue;
    for (int i = 3; i >= 0; i--)
        printf("%02x", pLong[i]);
}

static void print8longs(unsigned long v1, unsigned long v2, unsigned long v3, unsigned long v4, unsigned long v5, unsigned long v6, unsigned long v7, unsigned long v8)
{
    printlong(v1);
    printf(" ");
    printlong(v2);
    printf(" ");
    printlong(v3);
    printf(" ");
    printlong(v4);
    printf(" ");
    printlong(v5);
    printf(" ");
    printlong(v6);
    printf(" ");
    printlong(v7);
    printf(" ");
    printlong(v8);
    printf("\n");
}

static void prepare_input(unsigned long *pLongOutput, unsigned int iOutputSize, char *pInput, unsigned int iInputLength)
{
    for (int i = 0, j = 0; i < iInputLength; i+=4, j+=1)
    {
        long a = pInput[i];
        long b = (i+1) < iInputLength ? pInput[i+1] : 0x80;
        long c = (i+2) < iInputLength ? pInput[i+2] : b == 0x80 ? 0x00 : 0x80;
        long d = (i+3) < iInputLength ? pInput[i+3] : c == 0x80 ? 0x00 : 0x80;
        pLongOutput[j] = (a << 24) | (b << 16) | (c << 8) | d;
    }
    if (iInputLength % 4 == 0)
        pLongOutput[iInputLength/4] = 0x80000000;
    pLongOutput[iOutputSize-1] = iInputLength*8;
}

void long2char4(unsigned long lvalue, unsigned char *pvalue)
{
    for (int i = 0, j = 24; i < 4; i++, j -= 8)
        pvalue[i] = lvalue >> j;
}

Hash sha_create(char *pinput)
{
    if (pinput != NULL)
    {
        int len = 0;
        for(; pinput[len] != '\0'; len++);
        return sha_create2(pinput, len);
    }
    return 0;
}

Hash sha_create2(char *pinput, unsigned int length)
{
    _hash *phash = malloc(sizeof(_hash));
    sha256(pinput, length, phash);
    return (Hash)phash;
}

void sha_free(Hash hash)
{
    _hash *phash = (_hash*)hash;
    if (phash != NULL)
        free(phash);
}

char _ch2x(char ch)
{
    if (ch >= 0 && ch <= 9)
        return ch + '0';
    else if (ch >= 10 && ch <= 16)
        return ch - 10 + 'a';
    return ch;
}

void sha2xstr(Hash hash, char *str, size_t len)
{
    _hash *phash = (_hash*)hash;
    if (phash != NULL)
    {
        for (int i = 0, j = 0; i < HASH_LEN && j < len; i++, j+=2)
        {
            char x1 = _ch2x(phash->X[i] >> 4);
            char x2 = _ch2x(phash->X[i] & 0x0f);
            str[j] = x1;
            str[j+1] = x2;
        }
    }
}

void sha256(char *pInput, unsigned int iInputLength, _hash *p_hash)
{
    //printf("length:%d\n",iInputLength);
    unsigned long h1 = 0x6a09e667;
    unsigned long h2 = 0xbb67ae85;
    unsigned long h3 = 0x3c6ef372;
    unsigned long h4 = 0xa54ff53a;
    unsigned long h5 = 0x510e527f;
    unsigned long h6 = 0x9b05688c;
    unsigned long h7 = 0x1f83d9ab;
    unsigned long h8 = 0x5be0cd19;
    //print8longs(h1, h2, h3, h4, h5, h6, h7, h8);

    unsigned int isize = (iInputLength / 64 > 0) ? (iInputLength / 64 * 64 + 64) : 64;
    isize =  iInputLength % 64 >= 56 ? isize + 64 : isize;
    //printf("size:%d\n", isize);
    unsigned long *pPreparedInput = (unsigned long*)malloc(isize);
    prepare_input(pPreparedInput, isize/4, pInput, iInputLength);

    //printstr((char*)pPreparedInput, isize);

    for (int i = 0; i < isize/4; i = i + 16)
    {
        unsigned long W[64] = {0};
        for (int j = 0; j < 16; j++)
            W[j] = pPreparedInput[i+j];
        for (int j = 16; j < 64; j++)
            W[j] = Q1(W[j-2]) + W[j-7] + Q0(W[j-15]) + W[j-16];
        unsigned long a = h1;
        unsigned long b = h2;
        unsigned long c = h3;
        unsigned long d = h4;
        unsigned long e = h5;
        unsigned long f = h6;
        unsigned long g = h7;
        unsigned long h = h8; 
        for (int j = 0; j < 64; j++)
        {
            unsigned long t1 = h + E1(e) + CH(e, f, g) + K[j] + W[j];
            unsigned long t2 = E0(a) + MAJ(a, b, c);
            /*
            printf("h:");
            printlong(h);
            printf("\n");
            printf("E1(e):");
            printlong(E1(e));
            printf("\n");
            printf("CH(e,f,g):");
            printlong(CH(e,f,g));
            printf("\n");
            printf("K[%d]:", j);
            printlong(K[j]);
            printf("\n");
            printf("W[%d]:", j);
            printlong(W[j]);
            printf("\n");
            printf("T1");
            printlong(t1);
            printf("\n");
            */
            h = g;
            g = f;
            f = e;
            e = d + t1;
            d = c;
            c = b;
            b = a;
            a = t1 + t2;
            //printf("%d:", j);
            //print8longs(a, b, c, d, e, f, g, h);
        }
        h1 += a;
        h2 += b;
        h3 += c;
        h4 += d;
        h5 += e;
        h6 += f;
        h7 += g;
        h8 += h;
    }
    //print8longs(h1, h2, h3, h4, h5, h6, h7, h8);
    long2char4(h1, p_hash->X);
    long2char4(h2, p_hash->X+4);
    long2char4(h3, p_hash->X+8);
    long2char4(h4, p_hash->X+12);
    long2char4(h5, p_hash->X+16);
    long2char4(h6, p_hash->X+20);
    long2char4(h7, p_hash->X+24);
    long2char4(h8, p_hash->X+28);

    free(pPreparedInput);
}

