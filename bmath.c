#include "bmath.h"

/*
void printbin(Bin a)
{
    char szRet[66];
    bin2xstr2(a, szRet, 65);
    printf("%s\n", szRet);
}
*/

Bin bmath_gcd(Bin x, Bin y)
{
    Bin dx = bin_create3(x);
    Bin dy = bin_create3(y);
    Bin rem = bin_create3(y);
    Bin last_rem = 0;
    while (!bin_is_zero(rem))
    {
        bin_free(last_rem);
        last_rem = bin_create3(rem);
        Bin ret = bin_div_rem(dx, dy, &rem);
        bin_free(ret);
        bin_free(dx);
        dx = bin_create3(dy);
        bin_free(dy);
        dy = bin_create3(rem);
    }
    bin_free(dx);
    bin_free(dy);
    bin_free(rem);
    return last_rem;
}

// only work when gcd(a,m)=1
Bin bmath_inverse_mod(Bin a, Bin m)
{
    Bin u = bin_create3(a);
    Bin v = bin_create3(m);
    Bin x1 = bin_create_uint(1);
    Bin x2 = bin_create_uint(0);
    Bin zero = bin_create_uint(0);
    Bin one = bin_create_uint(1);
    while (bin_eq(u, one) > 0)
    {
        Bin r = 0;
        Bin q = bin_div_rem(v, u, &r);
        // x = x2 - q*x1
        bin_mul2(q, x1, &q);
        Bin x = bin_sub(x2, q);
        bin_free(q);
        bin_free(v);
        v = bin_create3(u);
        bin_free(u);
        u = bin_create3(r);
        bin_free(r);
        bin_free(x2);
        x2 = bin_create3(x1);
        bin_free(x1);
        x1 = bin_create3(x);
        bin_free(x);
    }
    // this is same as calling bin_mod(x1, m)
    Bin ret = bin_create3(x1);
    if (bin_eq(ret, zero) < 0)
        bin_add2(ret, m, &ret);
    bin_free(u);
    bin_free(v);
    bin_free(x1);
    bin_free(x2);
    bin_free(zero);
    bin_free(one);
    return ret;
}

void bmath_inverse_mod2(Bin a, Bin m, Bin *pr)
{
    Bin r = bmath_inverse_mod(a, m);
    bin_free(*pr);
    *pr = r;
}
