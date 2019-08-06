#include "ecc.h"
#include "bmath.h"
#include "mem_d.h"

#define _MALLOC(x) malloc_d(x, __FILE__, __LINE__)
#define _FREE free_d

typedef struct __ecc
{
    Bin a;
    Bin b;
} _ecc;

static void printbin(Bin a)
{
    char szStr[1024] = {0};
    bin2xstr(a, szStr, 1024);
    printf("%s\n", szStr);
}

_ecc* _ecc_create_uint(unsigned int a, unsigned int b)
{
    _ecc *pe = _MALLOC(sizeof(_ecc));
    pe->a = bin_create_uint(a);
    pe->b = bin_create_uint(b);
    return pe;
}

Ecc ecc_create_uint(unsigned int a, unsigned int b)
{
    _ecc *pe = _ecc_create_uint(a, b);
    return (Ecc)pe;
}

void _ecc_free(_ecc *pe)
{
    if (pe != NULL)
    {
        bin_free(pe->a);
        bin_free(pe->b);
        _FREE(pe);
    }
}

void ecc_free(Ecc ecc)
{
    _ecc *pe = (_ecc*)ecc;
    if (pe != NULL)
        _ecc_free(pe);
}

void _ecc_double(_ecc *pe, Bin x, Bin y, Bin m, Bin *rx, Bin *ry)
{
    if (pe != NULL)
    {
        Bin dx = bin_create3(x);
        Bin dy = bin_create3(y);
        Bin tmp1 = 0;
        Bin tmp2 = 0;
        Bin tmp3 = 0;
    
        Bin one = bin_create_uint(1);
        Bin two = bin_create("x2");
        Bin three = bin_create("x3");
        
        /*
        printf("x:");
        printbin(dx);
        printf("y:");
        printbin(dy);
        printf("m:");
        printbin(m);
        */

        // work out s
        // s = (3x^2+a)/2y
        // a = 3*x^2+a
        bin_mul2(dx, dx, &tmp1);
        //printf("x*x:");
        //printbin(tmp1);
        bin_mul2(three, tmp1, &tmp1);
        //printf("3*x^2:");
        //printbin(tmp1);
        bin_add2(tmp1, pe->a, &tmp1);
        //printf("3*x^2+a:");
        //printbin(tmp1);
        // b = 2*y
        bin_mul2(two, dy, &tmp2);
        //printf("2y:");
        //printbin(tmp2);
        // b^-1 mod n
        bmath_inverse_mod2(tmp2, m, &tmp2);
        //printf("b^-1 mod n:");
        //printbin(tmp2);
        // b^-1*a
        bin_mul2(tmp1, tmp2, &tmp1);
        //printf("a/b:");
        //printbin(tmp1);
        bin_mod2(tmp1, m, &tmp1);
        //printf("s:");
        //printbin(tmp1);
    
        // work out x
        // rx = s^2 - x - x
        bin_mul2(tmp1, tmp1, &tmp2);
        //printf("s^2:");
        //printbin(tmp2);
        bin_sub2(tmp2, dx, &tmp2);
        //printf("s^2-x:");
        //printbin(tmp2);
        bin_sub2(tmp2, dx, &tmp2);
        //printf("s^2-x-x:");
        //printbin(tmp2);
        bin_mod2(tmp2, m, rx);
        //printf("rx:");
        //printbin(*rx);
    
        // work out y
        // ry = s(x - rx) - y
        bin_sub2(dx, *rx, &tmp2);
        //printf("x-rx:");
        //printbin(tmp2);
        bin_mul2(tmp1, tmp2, &tmp2);
        //printf("s(x-rx):");
        //printbin(tmp2);
        bin_sub2(tmp2, dy, &tmp2);
        //printf("s(x-rx)-y:");
        //printbin(tmp2);
        bin_mod2(tmp2, m, ry);
        //printf("ry:");
        //printbin(*ry);
    
        bin_free(dx);
        bin_free(dy);
        bin_free(tmp1);
        bin_free(tmp2);
        bin_free(tmp3);
        bin_free(one);
        bin_free(two);
        bin_free(three);
    }
}

void ecc_double(Ecc ecc, Bin x, Bin y, Bin m, Bin *rx, Bin *ry)
{
    _ecc *pe = (_ecc*)ecc;
    if (pe != NULL && rx != NULL && ry != NULL)
    {
        if (*rx == 0)
            *rx = bin_create_uint(0);
        if (*ry == 0)
            *ry = bin_create_uint(0);
        _ecc_double(pe, x, y, m, rx, ry);
    }
}

void _ecc_add(_ecc *pe, Bin x1, Bin y1, Bin x2, Bin y2, Bin m, Bin *rx, Bin *ry)
{
    if (pe != NULL)
    {
        Bin dx1 = bin_create3(x1);
        Bin dy1 = bin_create3(y1);
        Bin dx2 = bin_create3(x2);
        Bin dy2 = bin_create3(y2);
        Bin tmp1 = 0;
        Bin tmp2 = 0;
    
        // work out s
        // s = (y2-y1)/(x2-x1)
        // a = y2-y1
        bin_sub2(dy2, dy1, &tmp1);
        //printf("y2-y1:");
        //printbin(tmp1);
        // b = x2-x1
        bin_sub2(dx2, dx1, &tmp2);
        bin_mod2(tmp2, m, &tmp2);  // do this because tmp2 could be negative
        //printf("x2-x1:");
        //printbin(tmp2);
        // b^-1 mod n
        bmath_inverse_mod2(tmp2, m, &tmp2);
        bin_mul2(tmp1, tmp2, &tmp1);
        bin_mod2(tmp1, m, &tmp1);
        //printf("s:");
        //printbin(tmp1);
    
        // work out x3
        // x3 = s^2 - x1 - x2
        bin_mul2(tmp1, tmp1, &tmp2);
        //printf("s^2:");
        //printbin(tmp2);
        bin_sub2(tmp2, dx1, &tmp2);
        //printf("s^2-x1:");
        //printbin(tmp2);
        bin_sub2(tmp2, dx2, &tmp2);
        //printf("s^2-x1-x2:");
        //printbin(tmp2);
        bin_mod2(tmp2, m, rx);
        //printf("x3:");
        //printbin(*rx);
    
        // work out y3
        // y3 = s(x1 - x3) - y1
        bin_sub2(dx1, *rx, &tmp2);
        //printf("x1-x3:");
        //printbin(tmp2);
        bin_mul2(tmp1, tmp2, &tmp1);
        //printf("s(x1-x3)");
        //printbin(tmp1);
        bin_sub2(tmp1, dy1, &tmp1);
        //printf("s(x1-x3)-y1:");
        //printbin(tmp1);
        bin_mod2(tmp1, m, ry);
        //printf("y3:");
        //printbin(*ry);
    
        bin_free(dx1);
        bin_free(dy1);
        bin_free(dx2);
        bin_free(dy2);
        bin_free(tmp1);
        bin_free(tmp2);
    }
}

void ecc_add(Ecc ecc, Bin x1, Bin y1, Bin x2, Bin y2, Bin m, Bin *rx, Bin *ry)
{
    _ecc *pe = (_ecc*)ecc;
    if (pe != NULL && rx != NULL && ry != NULL)
    {
        if (*rx == 0)
            *rx = bin_create_uint(0);
        if (*ry == 0)
            *ry = bin_create_uint(0);
        _ecc_add(pe, x1, y1, x2, y2, m, rx, ry);
    }
}

void _ecc_mul(_ecc *pe, Bin x, Bin y, Bin n, Bin m, Bin *rx, Bin *ry)
{
    Bin dx = bin_create3(x);
    Bin dy = bin_create3(y);

    char szBin[257] = {0};
    bin2bstr(n, szBin, 256);
    //printf("%s\n", szBin);
    int i = 0;
    for (; szBin[i] != '1'; i++);
    for (int j = 255; j >= i; j--)
    {
        if (szBin[j] == '1')
        {
            //printf("add\n");
            if (bin_is_zero(*rx) && bin_is_zero(*ry))
            {
                bin_free(*rx);
                bin_free(*ry);
                *rx = bin_create3(dx);
                *ry = bin_create3(dy);
            }
            else
            {
                _ecc_add(pe, *rx, *ry, dx, dy, m, rx, ry);
            }
        }
        //printf("double:\n");
        _ecc_double(pe, dx, dy, m, &dx, &dy);
        /*
        printf("x:");
        printbin(dx);
        printf("y:");
        printbin(dy);
        printf("rx:");
        printbin(*rx);
        printf("ry:");
        printbin(*ry);
        */
    }
    bin_free(dx);
    bin_free(dy);
}

void ecc_mul(Ecc ecc, Bin x, Bin y, Bin n, Bin m, Bin *rx, Bin *ry)
{
    _ecc *pe = (_ecc*)ecc;
    if (pe != NULL && rx != NULL && ry != NULL)
    {
        if (*rx == 0)
            *rx = bin_create_uint(0);
        if (*ry == 0)
            *ry = bin_create_uint(0);
        _ecc_mul(pe, x, y, n, m, rx, ry);
    }
}
