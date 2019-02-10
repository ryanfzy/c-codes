#include <string.h>
#include "binary.h"

#define BIN_LEN 32
#define CHAR_BITS 8
#define CHAR_NUM BIN_LEN/CHAR_BITS

typedef struct _bin32
{
    unsigned char cBin[CHAR_NUM];
} Bin32;

bool bin_init(Bin32*);
void bin_lshift(Bin32*, int);
void bin_rshift(Bin32*, int);
void bin_add2(Bin32*, Bin32*, Bin32*);
void bin_mul2(Bin32*, Bin32*, Bin32*);
void bin_sub2(Bin32*, Bin32*, Bin32*);
void bin_div2(Bin32*, Bin32*, Bin32*);

char _x2ch(char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    else if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    return ch;
}

void _bin_create_x(const char *pstr, unsigned int isize, Bin32 *pbin)
{
    int inext = isize;
    for (int i = 3; i >= 0 && inext >= 0; i--)
    {
        char ch1 = _x2ch((--inext >= 0) ? pstr[inext] : 0);
        char ch2 = _x2ch((--inext >= 0) ? pstr[inext] : 0);
        pbin->cBin[i] = (ch2 << 4 | ch1);
    }
}

Bin bin_create(const char *pstr, unsigned int isize)
{
    if (pstr != NULL)
    {
        Bin32 *pbin = malloc(sizeof(Bin32*));
        bin_init(pbin);
        if (pstr[0] == 'x')
            _bin_create_x(pstr+1, isize-1, pbin);
        return (Bin)pbin;
    }
    return 0;
}

void bin_free(Bin bin)
{
    Bin32 *pbin = (Bin32*)bin;
    free(pbin);
}

static Bin32 BINS[11] =
{
    {{0x00,0x00,0x00,0x00}},
    {{0x00,0x00,0x00,0x01}},
    {{0x00,0x00,0x00,0x02}},
    {{0x00,0x00,0x00,0x03}},
    {{0x00,0x00,0x00,0x04}},
    {{0x00,0x00,0x00,0x05}},
    {{0x00,0x00,0x00,0x06}},
    {{0x00,0x00,0x00,0x07}},
    {{0x00,0x00,0x00,0x08}},
    {{0x00,0x00,0x00,0x09}},
    {{0x00,0x00,0x00,0x0a}},
};

void bin_copy(Bin32 *pBTo, Bin32 *pBFrom)
{
    if (pBFrom != NULL && pBTo != NULL)
        *pBTo = *pBFrom;
}

bool bin_init(Bin32 *pBin32)
{
    if (pBin32 != NULL)
    {
        *pBin32 = BINS[0];
        return true;
    }
    return false;
}

void bin_lshift(Bin32 *pBin32, int iShift)
{
    if (pBin32 != NULL)
    {
        int iLshift = iShift % CHAR_BITS;
        int iRshift = CHAR_BITS - iLshift;
        for (int i = 0, j = (iShift / CHAR_BITS); i < CHAR_NUM; i++, j++)
        {
            if (BIN_LEN - i * CHAR_BITS > iShift)
            {
                unsigned char cShifted = pBin32->cBin[j] >> iRshift;
                pBin32->cBin[i] = pBin32->cBin[j] << iLshift;
                if (i > 0)
                    pBin32->cBin[i-1] |= cShifted;
            }
            else
                pBin32->cBin[i] &= 0x00;
        }
    }
}

bool bin_init_istr(Bin32 *pBin32, char *szStr, size_t iLen)
{
    if (pBin32 != NULL && szStr != NULL && iLen > 0)
    {
        bin_init(pBin32);
        for (int i = 0; i < iLen; i++)
        {
            int iBin32 = szStr[i] - '0';
            Bin32 binRet = *pBin32;
            bin_init(pBin32);
            if (iBin32 >= 0 && iBin32 <= 9)
            {
                if (i > 0)
                {
                    bin_mul2(&binRet, &(BINS[10]), pBin32);
                    binRet = *pBin32;
                }
                bin_add2(&binRet, &(BINS[iBin32]), pBin32);
            }
        }
        return true;
    }
    return false;
}

void bin_rshift(Bin32 *pBin32, int iShift)
{
    if (pBin32 != NULL)
    {
        int iRshift = iShift % CHAR_BITS;
        int iLshift = CHAR_BITS - iRshift;
        int iLastIndex = CHAR_NUM - 1;
        for (int i = iLastIndex, j = (iLastIndex - iShift / CHAR_BITS); i >= 0; i--, j--)
        {
            if ((i+1) * CHAR_BITS > iShift)
            {
                unsigned char cShifted = pBin32->cBin[j] << iLshift;
                pBin32->cBin[i] = pBin32->cBin[j] >> iRshift;
                if (i < iLastIndex)
                    pBin32->cBin[i+1] |= cShifted;
            }
            else
                pBin32->cBin[i] &= 0x00;
        }
    }
}

Bin bin_add(Bin a, Bin b)
{
    Bin32 *pa = (Bin32*)a;
    Bin32 *pb = (Bin32*)b;
    Bin32 *pr = (Bin32*)bin_create("x0", 2);
    bin_add2(pa, pb, pr);
    return (Bin)pr;
}

void bin_add2(Bin32 *pB1, Bin32 *pB2, Bin32 *pBRet)
{
    if (pB1 != NULL && pB2 != NULL && pBRet != NULL)
    {
        char chCarry = 0x00;
        for (int i = BIN_LEN - 1; i >= 0; i--)
        {
            int iIndex = i / CHAR_BITS;
            int iBitOffset = CHAR_BITS - (i % CHAR_BITS) - 1;

            char chCur = 0x00;
            int iMsk = 1 << iBitOffset;
            if ((pB1->cBin[iIndex] & iMsk) == (pB2->cBin[iIndex] & iMsk))
            {
                chCur = chCarry;
                chCarry = (pB1->cBin[iIndex] & iMsk) ? 0x01 : 0x00;
            }
            else
                chCur = (chCarry == 0x01) ? 0x00 : 0x01;
            if (chCur)
                pBRet->cBin[iIndex] |= (chCur << iBitOffset);
            else
                pBRet->cBin[iIndex] &= ~(1 << iBitOffset);
        }
    }
}

Bin bin_mul(Bin a, Bin b)
{
    Bin32 *pa = (Bin32*)a;
    Bin32 *pb = (Bin32*)b;
    Bin32 *pr = (Bin32*)bin_create("x0", 2);
    bin_mul2(pa, pb, pr);
    return (Bin)pr;
}

void bin_mul2(Bin32 *pB1, Bin32 *pB2, Bin32 *pBRet)
{
    if (pB1 != NULL && pB2 != NULL && pBRet != NULL)
    {
        for (int i = BIN_LEN -1 ; i >= 0; i--)
        {
            int iIndex = i / CHAR_BITS;
            int iBitOffset = CHAR_BITS - (i % CHAR_BITS) - 1;

            int iMsk = 1 << iBitOffset;
            if (pB2->cBin[iIndex] & iMsk)
            {
                Bin32 lastRet = *pBRet;
                bin_add2(&lastRet, pB1, pBRet);
            }
            bin_lshift(pB1, 1);
        }
    }
}

bool bin_eq(Bin32 *pB1, Bin32 *pB2)
{
    for (int i = 0; i < CHAR_NUM; i++)
    {
        if (pB1->cBin[i] != pB2->cBin[i])
            return false;
    }
    return true;
}

Bin bin_div(Bin a, Bin b)
{
    Bin32 *pa = (Bin32*)a;
    Bin32 *pb = (Bin32*)b;
    Bin32 *pr = (Bin32*)bin_create("x0", 2);
    bin_div2(pa, pb, pr);
    return (Bin)pr;
}

void bin_div2(Bin32 *pB1, Bin32 *pB2, Bin32 *pBRet)
{
    if (pB1 != NULL && pB2 != NULL && pBRet != NULL)
    {
        int iIndex = BIN_LEN-1;
        Bin32 bin1 = *pB1;
        while (!bin_eq(&bin1, &(BINS[0])))
        {
            int iBitOffset = CHAR_BITS - (iIndex % CHAR_BITS) - 1;
            if (bin1.cBin[CHAR_NUM-1] & 1)
            {
                Bin32 binRet = {{0x00,0x00,0x00,0x00}};
                bin_sub2(&bin1, pB2, &binRet);
                bin1 = binRet;
                pBRet->cBin[iIndex/CHAR_BITS] |= 1 << iBitOffset;
            }
            else
            {
                iIndex = iIndex-1;
                bin_rshift(&bin1, 1);
            }
        }
    }
}

Bin bin_sub(Bin a, Bin b)
{
    Bin32 *pa = (Bin32*)a;
    Bin32 *pb = (Bin32*)b;
    Bin32 *pr = (Bin32*)bin_create("x0", 2);
    bin_sub2(pa, pb, pr);
    return (Bin)pr;
}

void bin_sub2(Bin32 *pB1, Bin32 *pB2, Bin32 *pBRet)
{
    if (pB1 != NULL && pB2 != NULL && pBRet != NULL)
    {
        Bin32 binCmplmnt;
        Bin32 bin2Cmplmnt = {{0x00,0x00,0x00,0x00}};
        for (int i = 0; i < CHAR_NUM; i++)
            binCmplmnt.cBin[i] = ~(pB2->cBin[i]);
        bin_add2(&binCmplmnt, &(BINS[1]), &bin2Cmplmnt);
        bin_add2(pB1, &bin2Cmplmnt, pBRet);
    }
}

void _bin_reverse_str(char *szStr, size_t iLen)
{
    int iLastIndex = iLen - 1;
    for (int i = 0; i < iLen / 2; i++)
    {
        int iIndexToSwap = iLastIndex-i;
        if (iIndexToSwap != i)
        {
            char ch = szStr[i];
            szStr[i] = szStr[iIndexToSwap];
            szStr[iIndexToSwap] = ch;
        }
    }
}

void bin2bstr(Bin bin, char *szStr, size_t iLen)
{
    Bin32 *pBin32 = (Bin32*)bin;
    if (pBin32 != NULL && szStr != NULL && iLen > 0)
    {
        for (int i = BIN_LEN - 1, j = 0; i >= 0; i--, j++)
        {
            int iIndex = i / CHAR_BITS;
            int iBitOffset = CHAR_BITS - (i % CHAR_BITS) - 1;

            if (j < iLen)
                szStr[j] = (pBin32->cBin[iIndex] & (1 << iBitOffset)) ? '1' : '0';
        }
    }
    _bin_reverse_str(szStr, iLen < BIN_LEN ? iLen : BIN_LEN);
}

/*
// float
void _fstr2bin(Bin32 *pBin32, char *szStr, size_t iLen)
{
    if (pBin32 != NULL && szStr != NULL && iLen > 0) {
        _bin_init(pBin32);

        Bin32 binNum;
        bin_init_istr(&binNum, szStr, iLen);

        Bin32 binDen;
        _bin_copy(&binDen, &(BINS[1]));
        for (int i = 0; i < iLen; i++)
            bin_mul(&binDen, &(BINS[10]), &binDen);

        for (int i = 0; i < BIN_LEN; i++)
        {
            bin_lshift(&binNum, 1);
            if (bin_eq(&binNum, &binDen))
            {
                pBin32->cBin[i] = '1';
                return;
            }
            else if (_bin_is_larger(&binNum, &binDen))
            {
                bin_sub(&binNum, &binDen, &binNum);
                pBin32->cBin[i] = '1';
            }
        }
    }
}

bool bin_init_fstr(Bin32 *pBin32, char *szFloat, size_t iLen)
{
    if (pBin32 != NULL && szFloat != NULL && iLen > 0)
    {
        Bin32 bin1, bin2;
        if (_fstr_get_parts(szFloat, iLen, &bin1, &bin2))
        {
            _bin_init(pBin32);
            _fstr_add_parts(&bin1, &bin2, pBin32);
        }
    }
    return false;
}
bool _fstr_get_parts(char *szFloat, size_t iLen, Bin32 *pB1, Bin32 *pB2)
{
    if (szFloat != NULL && iLen > 0 && pB1 != NULL && pB2 != NULL)
    {
        int iPoint = iLen;
        for (int i = 0; i < iLen; i++)
        {
            if (szFloat[i] == '.')
            {
                iPoint = i;
                break;
            }
        }

        bin_init_istr(pB1, szFloat, iPoint);

        if (iPoint < iLen)
            _fstr2bin(pB2, szFloat+iPoint+1, iLen-iPoint-1);
        return true;
    }
    return false;
}

int _bin_get1pos(Bin32 *pBin32)
{
    if (pBin32 != NULL)
    {
        for (int i = 0; i < BIN_LEN; i++)
        {
            if (pBin32->cBin[i] == '1')
                return i;
        }
    }
    return -1;
}

void _fstr_get_mantissa(Bin32 *pB1, Bin32 *pB2, Bin32 *pBRet, int *piExp)
{
    if (pB1 != NULL && pB2 != NULL && pBRet != NULL)
    {
        int iExp = 127;
        int iPos = _bin_get1pos(pB1);
        if (iPos >= 0)
        {
            iExp += (BIN_LEN-iPos-1);
            bin_lshift(pB1, iPos+1);
        }
        iPos = _bin_get1pos(pB2);
        if (iPos >= 0)
        {
            if (iExp == 127)
            {
                bin_lshift(pB2, iPos+1);
                iExp = 127-iPos-1;
            }
            else
                bin_rshift(pB2, iExp - 127);
        }

        bin_add(pB1, pB2, pBRet);

        if (piExp != NULL)
            *piExp = iExp;
    }
}

void _fstr_add_parts(Bin32 *pB1, Bin32 *pB2, Bin32 *pBRet)
{
    if (pB1 != NULL && pB2 != NULL && pBRet != NULL)
    {
        int iExp = 0;
        _fstr_get_mantissa(pB1, pB2, pBRet, &iExp);

        bool bMoreValue = false;
        for (int i = 23; i < BIN_LEN; i++)
        {
            if (pBRet->cBin[i] == '1')
            {
                bMoreValue = true;
                break;
            }
        }
        bin_rshift(pBRet, 9);
        if (bMoreValue)
            pBRet->cBin[31] = '1';

        Bin32 binExp;
        _bin_init(&binExp);
        for (int i = 0; i < iExp; i++)
            bin_add(&binExp, &(BINS[1]), &binExp);
        bin_lshift(&binExp, 23);
        bin_add(pBRet, &binExp, pBRet);
    }
}
*/
