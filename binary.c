#include <string.h>
#include "binary.h"

static Bin BINS[11] = 
{
    {"00000000000000000000000000000000"},
    {"00000000000000000000000000000001"},
    {"00000000000000000000000000000010"},
    {"00000000000000000000000000000011"},
    {"00000000000000000000000000000100"},
    {"00000000000000000000000000000101"},
    {"00000000000000000000000000000110"},
    {"00000000000000000000000000000111"},
    {"00000000000000000000000000001000"},
    {"00000000000000000000000000001001"},
    {"00000000000000000000000000001010"},
};

bool _bin_init(Bin *pBin)
{
    if (pBin != NULL)
    {
        for (int i = 0; i < 32; i++)
            pBin->cBin[i] = '0';
        return true;
    }
    return false;
}

void _bin_print(Bin *pBin)
{
    if (pBin != NULL)
    {
        char szBin[33] = {0};
        memcpy(szBin, pBin->cBin, 32);
        printf(szBin);
    }
}

bool bin_init_bstr(Bin *pBin, char *szBin, size_t iLen)
{
    if (pBin != NULL)
    {
        for (int i = 31, j = iLen - 1; i >= 0; i--, j--)
        {
            if (szBin != NULL && j >= 0)
                pBin->cBin[i] = szBin[j];
            else
                pBin->cBin[i] = '0';
        }
        return true;
    }
    return false;
}

void _bin_copy(Bin *pBTo, Bin *pBFrom)
{
    if (pBFrom != NULL && pBTo != NULL)
    {
        for (int i = 0; i < 32; i++)
            pBTo->cBin[i] = pBFrom->cBin[i];
    }
}


bool _ch2bin(Bin *pBin, char ch)
{
    bool bRet = false;
    if (pBin != NULL)
    {
        int iBin = ch - '0';
        if (iBin < 10)
        {
            _bin_copy(pBin, &BINS[iBin]);
            bRet = true;
        }
    }
    return bRet;
}

bool bin_init_istr(Bin *pBin, char *szStr, size_t iLen)
{
    if (pBin != NULL && szStr != NULL && iLen > 0)
    {
        _bin_init(pBin);
        for (int i = 0; i < iLen; i++)
        {
            Bin curBin;
            _bin_init(&curBin);
            if (_ch2bin(&curBin, szStr[i]))
            {
                Bin tmpRet;
                _bin_copy(&tmpRet, pBin);
                if (i > 0)
                {
                    _bin_init(&tmpRet);
                    bin_mul(pBin, &BINS[10], &tmpRet);
                }
                bin_add(&tmpRet, &curBin, pBin);
            }
        }
        return true;
    }
    return false;
}

/*
void _fstr2bin(Bin *pBin, char *szStr, size_t iLen)
{
}

void fstr2bin(Bin *pBin, char *szStr, size_t iLen)
{
    if (pBin != NULL && szStr != NULL && iLen > 0)
    {
        int iDecimalPoint = 0;
        for (int i = 0; i < iLen; i++)
        {
            if (szStr[i] == '.')
            {
                iDecimalPoint = i;
                break;
            }
        }

        Bin binPartOne, binPartTwo;
        bin_init(&binPartOne);
        bin_init(&binPartTwo);
        istr2bin(&binPartOne, szStr, iDecimalPoint);
        _fstr2bin(&binPartTwo, szStr[iDecimalPoint+1], iLen-iDecimalPoint);
    }
}
*/

void bin_add(Bin *pB1, Bin *pB2, Bin *pBRet)
{
    if (pB1 != NULL && pB2 != NULL && pBRet)
    {
        char chCarry = '0';
        for (int i = 31; i >= 0; i--)
        {
            char chCur = '0';
            if (pB1->cBin[i] == pB2->cBin[i])
            {
                chCur = chCarry;
                chCarry = pB1->cBin[i];
            }
            else
                chCur = chCarry == '1' ? '0' : '1';
            pBRet->cBin[i] = chCur;
        }
    }
}


void _bin_lshift(Bin *pBin, int iShift)
{
    if (pBin != NULL)
    {
        for (int i = 0, j = iShift; i < 32; i++, j++)
        {
            if (j < 32)
                pBin->cBin[i] = pBin->cBin[j];
            else
                pBin->cBin[i] = '0';
        }
    }
}

void _bin_rshift(Bin *pBin, int iShift)
{
    if (pBin != NULL)
    {
        for (int i = 31, j = 31-iShift; i >= 0; i--, j--)
        {
            if (j >= 0)
                pBin->cBin[i] = pBin->cBin[j];
            else
                pBin->cBin[i] = '0';
        }
    }
}

void bin_mul(Bin *pB1, Bin *pB2, Bin *pBRet)
{
    if (pB1 != NULL && pB2 != NULL && pBRet != NULL)
    {
        for (int i = 31, j = 0; i >= 0; i--, j++)
        {
            if (pB2->cBin[i] == '1')
            {
                Bin bin, ret;
                _bin_copy(&bin, pB1);
                _bin_lshift(&bin, j);
                bin_add(&bin, pBRet, &ret);
                _bin_copy(pBRet, &ret);
            }
        }
    }
}

bool bin_eq(Bin *pB1, Bin *pB2)
{
    if (pB1 != NULL && pB2 != NULL)
    {
        for (int i = 0; i < 32; i++)
        {
            if (pB1->cBin[i] != pB2->cBin[i])
                return false;
        }
        return true;
    }
    return false;
}

void bin_div(Bin *pB1, Bin *pB2, Bin *pBRet)
{
    if (pB1 != NULL && pB2 != NULL && pBRet != NULL)
    {
        Bin binRem;
        _bin_copy(&binRem, pB1);
        int iIndex = 31;
        while (!bin_eq(&binRem, pB2))
        {
            if (binRem.cBin[31] == '1')
            {
                Bin bin;
                _bin_copy(&bin, &binRem);
                bin_sub(&bin, pB2, &binRem);
                pBRet->cBin[iIndex] = '1';
            }
            else
            {
                iIndex = iIndex-1;
                _bin_rshift(&binRem, 1);
            }
        }
        pBRet->cBin[iIndex] = '1';
    }
}

void _bin2cmplmnt(Bin *pBin)
{
    if (pBin != NULL)
    {
        for (int i = 0; i < 32; i++)
            pBin->cBin[i] = pBin->cBin[i] == '1' ? '0' : '1';
    }
}

void _bin_get_2cmplmnt(Bin *pTo, Bin *pFrom)
{
    if (pTo != NULL && pFrom != NULL)
    {
        Bin binCmplmnt;
        _bin_init(&binCmplmnt);

        _bin_copy(&binCmplmnt, pFrom);
        _bin2cmplmnt(&binCmplmnt);
        bin_add(&binCmplmnt, &BINS[1], pTo);
    }
}

void bin_sub(Bin *pB1, Bin *pB2, Bin *pBRet)
{
    // here use 2's complement method
    if (pB1 != NULL && pB2 != NULL && pBRet != NULL)
    {
        Bin bin2cmplmnt;
        _bin_get_2cmplmnt(&bin2cmplmnt, pB2);
        bin_add(pB1, &bin2cmplmnt, pBRet);
    }
}

void _bin2str(Bin *pBin, char *szStr, size_t iSize)
{
    if (pBin != NULL && szStr != NULL && iSize > 0)
    {
        bool bStartCpy = false;
        int iDigitCopyed = 0;
        for (int i = 0; i < 32; i++)
        {
            if (!bStartCpy && pBin->cBin[i] == '1')
                bStartCpy = true;
            if (bStartCpy && iDigitCopyed <= iSize)
            {
                szStr[iDigitCopyed] = pBin->cBin[i];
                iDigitCopyed++;
            }
        }
    }
}


void bin_add_ex(char *szB1, size_t iLen1, char *szB2, size_t iLen2, char *szRet, size_t iLenRet)
{
    Bin b1, b2, ret;
    if (bin_init_bstr(&b1, szB1, iLen1) && bin_init_bstr(&b2, szB2, iLen2) && _bin_init(&ret))
    {
        bin_add(&b1, &b2, &ret);
        _bin2str(&ret, szRet, iLenRet);
    }
}

void bin_mul_ex(char *szB1, size_t iLen1, char *szB2, size_t iLen2, char *szRet, size_t iLenRet)
{
    Bin b1, b2, ret;
    if (bin_init_bstr(&b1, szB1, iLen1) && bin_init_bstr(&b2, szB2, iLen2) && _bin_init(&ret))
    {
        bin_mul(&b1, &b2, &ret);
        _bin2str(&ret, szRet, iLenRet);
    }
}

void bin_sub_ex(char *szB1, size_t iLen1, char *szB2, size_t iLen2, char *szRet, size_t iLenRet)
{
    Bin b1, b2, ret;
    if (bin_init_bstr(&b1, szB1, iLen1) && bin_init_bstr(&b2, szB2, iLen2) && _bin_init(&ret))
    {
        bin_sub(&b1, &b2, &ret);
        _bin2str(&ret, szRet, iLenRet);
    }
}

void bin_div_ex(char *szB1, size_t iLen1, char *szB2, size_t iLen2, char *szRet, size_t iLenRet)
{
    Bin b1, b2, ret;
    if (bin_init_bstr(&b1, szB1, iLen1) && bin_init_bstr(&b2, szB2, iLen2) && _bin_init(&ret))
    {
        bin_div(&b1, &b2, &ret);
        _bin2str(&ret, szRet, iLenRet);
    }
}

void int2bin_ex(char *szInt, size_t iLenInt, char *szBin, size_t iLenBin)
{
    if (szInt != NULL && iLenInt > 0 && szBin != NULL && iLenBin > 0)
    {
        Bin bin;
        bin_init_istr(&bin, szInt, iLenInt);
        _bin2str(&bin, szBin, iLenBin);
    }
}
