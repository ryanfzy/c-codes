#include <string.h>
#include "binary.h"

//#define BIN_LEN 32
#define BIN_LEN 256
#define CHAR_BITS 8
#define CHAR_NUM BIN_LEN/CHAR_BITS

typedef struct _bin32
{
    unsigned char cBin[CHAR_NUM];
} _bin;

bool bin_init(_bin*);
void bin_lshift(_bin*, int);
void bin_rshift(_bin*, int);
void _bin_add(_bin*, _bin*, _bin*);
void _bin_mul(_bin*, _bin*, _bin*);
void _bin_sub(_bin*, _bin*, _bin*);
void _bin_div(_bin*, _bin*, _bin*);
void bin_printb(_bin*);
void bin_printx(_bin*);

char _x2ch(char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    else if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    return ch;
}

char _ch2x(char ch)
{
    if (ch >= 0 && ch <= 9)
        return ch + '0';
    else if (ch >= 10 && ch <= 16)
        return ch - 10 + 'a';
    return ch;
}

void _bin_create_x(const char *pstr, unsigned int isize, _bin *pbin)
{
    int inext = isize;
    for (int i = CHAR_NUM-1; i >= 0 && inext >= 0; i--)
    {
        char ch1 = _x2ch((--inext >= 0) ? pstr[inext] : 0);
        char ch2 = _x2ch((--inext >= 0) ? pstr[inext] : 0);
        pbin->cBin[i] = (ch2 << 4 | ch1);
    }
}

Bin bin_create(const char *pstr)
{
    if (pstr != NULL)
    {
        int len = 0;
        for(; pstr[len] != '\0'; len++);
        return bin_create2(pstr, len);
    }
    return 0;
}

Bin bin_create2(const char *pstr, unsigned int isize)
{
    if (pstr != NULL)
    {
        _bin *pbin = malloc(sizeof(_bin));
        bin_init(pbin);
        if (pstr[0] == 'x')
            _bin_create_x(pstr+1, isize-1, pbin);
        return (Bin)pbin;
    }
    return 0;
}

void bin_free(Bin bin)
{
    _bin *pbin = (_bin*)bin;
    free(pbin);
}

void bin_copy(_bin *pBTo, _bin *pBFrom)
{
    if (pBFrom != NULL && pBTo != NULL)
        *pBTo = *pBFrom;
}

bool bin_init(_bin *pbin)
{
    if (pbin != NULL)
    {
        for (int i = 0; i < CHAR_NUM; i++)
            pbin->cBin[i] = 0;
        return true;
    }
    return false;
}

void bin_lshift(_bin *pbin, int iShift)
{
    if (pbin != NULL)
    {
        int iLshift = iShift % CHAR_BITS;
        int iRshift = CHAR_BITS - iLshift;
        for (int i = 0, j = (iShift / CHAR_BITS); i < CHAR_NUM; i++, j++)
        {
            if (BIN_LEN - i * CHAR_BITS > iShift)
            {
                unsigned char cShifted = pbin->cBin[j] >> iRshift;
                pbin->cBin[i] = pbin->cBin[j] << iLshift;
                if (i > 0)
                    pbin->cBin[i-1] |= cShifted;
            }
            else
                pbin->cBin[i] &= 0x00;
        }
    }
}

/*
bool bin_init_istr(_bin *pbin, char *szStr, size_t iLen)
{
    if (pbin != NULL && szStr != NULL && iLen > 0)
    {
        bin_init(pbin);
        for (int i = 0; i < iLen; i++)
        {
            int i_bin = szStr[i] - '0';
            _bin binRet = *pbin;
            bin_init(pbin);
            if (i_bin >= 0 && i_bin <= 9)
            {
                if (i > 0)
                {
                    _bin_mul(&binRet, &(BINS[10]), pbin);
                    binRet = *pbin;
                }
                _bin_add(&binRet, &(BINS[i_bin]), pbin);
            }
        }
        return true;
    }
    return false;
}
*/

void bin_rshift(_bin *pbin, int iShift)
{
    if (pbin != NULL)
    {
        int iRshift = iShift % CHAR_BITS;
        int iLshift = CHAR_BITS - iRshift;
        int iLastIndex = CHAR_NUM - 1;
        for (int i = iLastIndex, j = (iLastIndex - iShift / CHAR_BITS); i >= 0; i--, j--)
        {
            if ((i+1) * CHAR_BITS > iShift)
            {
                unsigned char cShifted = pbin->cBin[j] << iLshift;
                pbin->cBin[i] = pbin->cBin[j] >> iRshift;
                if (i < iLastIndex)
                    pbin->cBin[i+1] |= cShifted;
            }
            else
                pbin->cBin[i] &= 0x00;
        }
    }
}

Bin bin_add(Bin a, Bin b)
{
    _bin *pa = (_bin*)a;
    _bin *pb = (_bin*)b;
    _bin *pr = (_bin*)bin_create("x0");
    _bin_add(pa, pb, pr);
    return (Bin)pr;
}

void _bin_add(_bin *pB1, _bin *pB2, _bin *pBRet)
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
    _bin *pa = (_bin*)a;
    _bin *pb = (_bin*)b;
    _bin *pr = (_bin*)bin_create("x0");
    _bin_mul(pa, pb, pr);
    return (Bin)pr;
}

void _bin_mul(_bin *pB1, _bin *pB2, _bin *pBRet)
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
                _bin lastRet = *pBRet;
                _bin_add(&lastRet, pB1, pBRet);
            }
            bin_lshift(pB1, 1);
        }
    }
}

int bin_eq(_bin *pB1, _bin *pB2)
{
    for (int i = 0; i < CHAR_NUM; i++)
    {
        if (pB1->cBin[i] > pB2->cBin[i])
            return 1;
        else if (pB1->cBin[i] < pB2->cBin[i])
            return -1;
    }
    return 0;
}

int _bin_get_startIndex(_bin *pa)
{
    if (pa != NULL)
    {
        for (int i = 0; i < CHAR_NUM; i++)
        {
            if (pa->cBin[i] > 0)
            {
                for (int j = 7; j >= 0; j--)
                {
                    if (pa->cBin[i] & (1 << j))
                        return i * CHAR_BITS + 8-j-1;
                }
            }
        }
    }
    return -1;
}

void _bin_align(_bin *pa, _bin *pb, _bin *pr)
{
    if (pa != NULL && pb != NULL && pr != NULL)
    {
        int ia = _bin_get_startIndex(pa);
        int ib = _bin_get_startIndex(pb);
        bin_copy(pr, pb);
        bin_lshift(pr, ib - ia);
    }
}

Bin bin_div(Bin a, Bin b)
{
    _bin *pa = (_bin*)a;
    _bin *pb = (_bin*)b;
    _bin *pr = (_bin*)bin_create("x0");
    _bin_div(pa, pb, pr);
    return (Bin)pr;
}

void _bin_div(_bin *pB1, _bin *pB2, _bin *pBRet)
{
    if (pB1 != NULL && pB2 != NULL && pBRet != NULL)
    {
        _bin *paligned = (_bin*)bin_create("x0");
        _bin_align(pB1, pB2, paligned);
        _bin a = *pB1;
        while (bin_eq(paligned, pB2) >= 0)
        {
            bin_lshift(pBRet, 1);
            if (bin_eq(&a, paligned) >= 0)
            {
                _bin bin;
                bin_init(&bin);
                _bin_sub(&a, paligned, &bin);
                a = bin;
                pBRet->cBin[CHAR_NUM-1] |= 0x01;
            }
            bin_rshift(paligned, 1);
        }
        bin_free((Bin)paligned);
    }
}

Bin bin_sub(Bin a, Bin b)
{
    _bin *pa = (_bin*)a;
    _bin *pb = (_bin*)b;
    _bin *pr = (_bin*)bin_create("x0");
    _bin_sub(pa, pb, pr);
    return (Bin)pr;
}

void _bin_sub(_bin *pB1, _bin *pB2, _bin *pBRet)
{
    if (pB1 != NULL && pB2 != NULL && pBRet != NULL)
    {
        _bin* pOne = (_bin*)bin_create("x1");
        _bin binCmplmnt;
        _bin bin2Cmplmnt;
        bin_init(&binCmplmnt);
        for (int i = 0; i < CHAR_NUM; i++)
            binCmplmnt.cBin[i] = ~(pB2->cBin[i]);
        _bin_add(&binCmplmnt, pOne, &bin2Cmplmnt);
        _bin_add(pB1, &bin2Cmplmnt, pBRet);
        bin_free((Bin)pOne);
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

void bin_printb(_bin *pbin)
{
    char szStr[257] = {0};
    bin2bstr((Bin)pbin, szStr, 256);
    int i = 0;
    for (; i < 257; i++)
    {
        if (szStr[i] == '1')
            break;
    }
    printf("%10s\n", szStr+i);
}

void bin_printx(_bin *pa)
{
    char szStr[65] = {0};
    bin2xstr((Bin)pa, szStr, 64);
    printf("%s\n", szStr);
}

void bin2bstr(Bin bin, char *szStr, size_t iLen)
{
    _bin *pbin = (_bin*)bin;
    if (pbin != NULL && szStr != NULL && iLen > 0)
    {
        for (int i = BIN_LEN - 1, j = 0; i >= 0; i--, j++)
        {
            int iIndex = i / CHAR_BITS;
            int iBitOffset = CHAR_BITS - (i % CHAR_BITS) - 1;

            if (j < iLen)
                szStr[j] = (pbin->cBin[iIndex] & (1 << iBitOffset)) ? '1' : '0';
        }
    }
    _bin_reverse_str(szStr, iLen < BIN_LEN ? iLen : BIN_LEN);
}

void bin2xstr(Bin bin, char *szStr, size_t iLen)
{
    _bin *pbin = (_bin*)bin;
    if (pbin != NULL && szStr != NULL && iLen > 0)
    {
        for (int i = CHAR_NUM-1, j = 0; i >= 0 && j < iLen; i--, j+=2)
        {
            char ch1 = _ch2x(pbin->cBin[i] >> 4);
            char ch2 = _ch2x(pbin->cBin[i] & 0x0f);
            szStr[j] = ch2;
            szStr[j+1] = ch1;
        }
        _bin_reverse_str(szStr, iLen < BIN_LEN ? iLen : BIN_LEN);
    }
}

/*
// float
void _fstr2bin(_bin *pbin, char *szStr, size_t iLen)
{
    if (pbin != NULL && szStr != NULL && iLen > 0) {
        _bin_init(pbin);

        _bin binNum;
        bin_init_istr(&binNum, szStr, iLen);

        _bin binDen;
        _bin_copy(&binDen, &(BINS[1]));
        for (int i = 0; i < iLen; i++)
            bin_mul(&binDen, &(BINS[10]), &binDen);

        for (int i = 0; i < BIN_LEN; i++)
        {
            bin_lshift(&binNum, 1);
            if (bin_eq(&binNum, &binDen))
            {
                pbin->cBin[i] = '1';
                return;
            }
            else if (_bin_is_larger(&binNum, &binDen))
            {
                bin_sub(&binNum, &binDen, &binNum);
                pbin->cBin[i] = '1';
            }
        }
    }
}

bool bin_init_fstr(_bin *pbin, char *szFloat, size_t iLen)
{
    if (pbin != NULL && szFloat != NULL && iLen > 0)
    {
        _bin bin1, bin2;
        if (_fstr_get_parts(szFloat, iLen, &bin1, &bin2))
        {
            _bin_init(pbin);
            _fstr_add_parts(&bin1, &bin2, pbin);
        }
    }
    return false;
}
bool _fstr_get_parts(char *szFloat, size_t iLen, _bin *pB1, _bin *pB2)
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

int _bin_get1pos(_bin *pbin)
{
    if (pbin != NULL)
    {
        for (int i = 0; i < BIN_LEN; i++)
        {
            if (pbin->cBin[i] == '1')
                return i;
        }
    }
    return -1;
}

void _fstr_get_mantissa(_bin *pB1, _bin *pB2, _bin *pBRet, int *piExp)
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

void _fstr_add_parts(_bin *pB1, _bin *pB2, _bin *pBRet)
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

        _bin binExp;
        _bin_init(&binExp);
        for (int i = 0; i < iExp; i++)
            bin_add(&binExp, &(BINS[1]), &binExp);
        bin_lshift(&binExp, 23);
        bin_add(pBRet, &binExp, pBRet);
    }
}
*/
