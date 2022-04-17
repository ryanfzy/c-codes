#include "secp256k1.h"
#include "secp256k1table.h"
#include "ecc.h"
#include "bmath.h"

static void _ecc_mul(Ecc ecc, Bin k, Bin m, Bin *rx, Bin *ry);

static void printbin(Bin a)
{
    char szStr[1024] = {0};
    bin2xstr(a, szStr, 1024);
    printf("%s\n", szStr);
}

void sign(Bin d, Bin h, Bin *pr, Bin *ps)
{
    if (pr != NULL && ps != NULL)
    {
        // use a fix table to improve performance, however, it is still too slow
        secp256k1table_init();

        Bin P = bin_create("xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f");
        Bin GX = bin_create("x79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798");
        Bin GY = bin_create("x483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8");
        Bin N = bin_create("xfffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141");
        // k must be a random number
        //Bin K = bin_create("xfedcba9876543210fedcba9876543210fedcba9876543210fedcba9876543210");
        Bin K = bin_create_uint(123);
        printf("x: ");
        printbin(GX);
        printf("y: ");
        printbin(GY);
        printf("k: ");
        printbin(K);
    
        /*
        Ecc ecc = ecc_create_uint(0, 7);
        Bin x = 0;
        Bin y = 0;
        _ecc_mul(ecc, K, P, &x, &y);
        */
        // for debug only
        // kG = 123*G
        Bin x = bin_create("xa598a8030da6d86c6bc7f2f5144ea549d28211ea58faa70ebf4c1e665c1fe9b5");
        Bin y = bin_create("x204b5d6f84822c307e4b4a7140737aec23fc63b65b35f86a10026dbd2d864e6b");
    
        printf("px: ");
        printbin(x);
        printf("py: ");
        printbin(y);
    
        bin_mod2(x, N, pr);
        printf("r:");
        printbin(*pr);
    
        Bin tmp1 = 0;
        Bin tmp2 = 0;
        bin_mul2(d, *pr, &tmp1);
        printf("dr:");
        printbin(tmp1);
        bin_add2(h, tmp1, &tmp1);
        printf("dr+h:");
        printbin(tmp1);
        bmath_inverse_mod2(K, N, &tmp2);
        printf("k^-1:");
        printbin(tmp2);
        bin_mul2(tmp1, tmp2, &tmp1);
        bin_mod2(tmp1, N, ps);
        printf("s: ");
        printbin(*ps);

        bin_free(tmp1);
        bin_free(tmp2);
        bin_free(x);
        bin_free(y);
        //ecc_free(ecc);
        bin_free(P);
        bin_free(GX);
        bin_free(GY);
        bin_free(N);
        bin_free(K);

        secp256k1table_free();
    }
}

bool verify(Bin x, Bin y, Bin r, Bin s, Bin h)
{
    Ecc ecc = ecc_create_uint(0, 7);
    Bin P = bin_create("xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f");
    Bin GX = bin_create("x79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798");
    Bin GY = bin_create("x483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8");
    Bin N = bin_create("xfffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141");
    printf("P:");
    printbin(P);
    printf("GX:");
    printbin(GX);
    printf("GY:");
    printbin(GY);
    printf("N:");
    printbin(N);
    printf("X:");
    printbin(x);
    printf("Y:");
    printbin(y);
    printf("s:");
    printbin(s);
    printf("r:");
    printbin(r);
    printf("h:");
    printbin(h);
    Bin tmp1 = 0;
    Bin tmp2 = 0;
    Bin x1 = 0;
    Bin y1 = 0;
    Bin x2 = 0;
    Bin y2 = 0;
    bmath_inverse_mod2(s, N, &tmp1);
    printf("w:");
    printbin(tmp1);
    bin_mul2(h, tmp1, &tmp2);
    bin_mod2(tmp2, N, &tmp2);
    printf("u1:");
    printbin(tmp2);
    bin_mul2(r, tmp1, &tmp1);
    bin_mod2(tmp1, N, &tmp1);
    printf("u2:");
    printbin(tmp1);
    _ecc_mul(ecc, tmp2, P, &x1, &y1);
    printf("u1x:");
    printbin(x1);
    printf("u1y:");
    printbin(y1);
    ecc_mul(ecc, x, y, tmp1, P, &x2, &y2);
    printf("u2x:");
    printbin(x2);
    printf("u2y:");
    printbin(y2);
    ecc_add(ecc, x1, y1, x2, y2, P, &x1, &y1);
    bin_mod2(x1, N, &x1);
    printf("x2:");
    printbin(x1);
    bool ret = bin_eq(r, x1) == 0;
    bin_free(P);
    bin_free(GX);
    bin_free(GY);
    bin_free(N);
    bin_free(tmp1);
    bin_free(tmp2);
    bin_free(x1);
    bin_free(y1);
    bin_free(x2);
    bin_free(y2);
    ecc_free(ecc);
    return ret;
}

static void _ecc_mul(Ecc ecc, Bin k, Bin m, Bin *rx, Bin *ry)
{
    char szBin[257] = {0};
    bin2bstr(k, szBin, 256);
    //printf("%s\n", szBin);
    int i = 0;
    for (; szBin[i] != '1'; i++);
    for (int j = 255, k = 0; j >= i; j--, k++)
    {
        if (szBin[j] == '1')
        {
            //printf("add\n");
            Bin x = secp256k1table_x(k);
            Bin y = secp256k1table_y(k);
            if (bin_is_zero(*rx) && bin_is_zero(*ry))
            {
                bin_free(*rx);
                bin_free(*ry);
                *rx = bin_create3(x);
                *ry = bin_create3(y);
            }
            else
            {
                ecc_add(ecc, *rx, *ry, x, y, m, rx, ry);
            }
            /*
            printf("x:");
            printbin(*rx);
            printf("y:");
            printbin(*ry);
            */
        }
        /*
        printf("rx:");
        printbin(*rx);
        printf("ry:");
        printbin(*ry);
        */
    }
}
