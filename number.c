#include <string.h>
#include "number.h"

// only support 36 base
static const char *pBaseStr = "0123456789abcdefghijklmnopqrstuvwxyz";

static void _str_reverse(char *szStr, int iLeftIndex, int iRightIndex)
{
    if (iLeftIndex < iRightIndex)
    {
        char ch = szStr[iLeftIndex];
        szStr[iLeftIndex] = szStr[iRightIndex];
        szStr[iRightIndex] = ch;
        _str_reverse(szStr, iLeftIndex+1, iRightIndex-1);
    }
}

static void str_reverse(char *szStr, size_t iLen)
{
    _str_reverse(szStr, 0, iLen-1);
}

static int _int2str(int iNum, int iBase, char *szStr, size_t iLen, int iIndex, bool bIncIndex)
{
    if (bIncIndex ? iIndex < iLen : iIndex >= 0)
    {
        if (iNum < iBase)
        {
            szStr[iIndex] = pBaseStr[iNum];
            iIndex = bIncIndex ? iIndex+1 : iIndex-1;
        }
        else
        {
            int iRemainder = iNum % iBase;
            szStr[iIndex] = pBaseStr[iRemainder];
            int iReduced = (iNum - iRemainder) / iBase;
            iIndex = _int2str(iReduced, iBase, szStr, iLen, bIncIndex ? iIndex+1 : iIndex-1, bIncIndex);
        }
    }
    return iIndex;
}

void int2str(int iNum, int iBase, char *szStr, size_t iLen)
{
    if (iNum > -1 && iBase > 0 && iBase < 36 && szStr != NULL && iLen > 0)
    {
        int iStrLen = _int2str(iNum, iBase, szStr, iLen, 0, true);
        str_reverse(szStr, iStrLen);
    }
}

static int _ch2int(char ch)
{
    for (int i = 0; i < 36; i++)
    {
        if (ch == pBaseStr[i])
            return i;
    }
    return -1;
}

static int _str2int(char *szStr, int iLastIndex, int iBase)
{
    if (iLastIndex > -1)
    {
        int iNum = _ch2int(szStr[iLastIndex]);
        if (iNum > -1)
            return iNum + _str2int(szStr, iLastIndex-1, iBase) * iBase;
    }
    return 0;
}

int str2int(char *szStr, size_t iLen, int iBase)
{
    if (szStr != NULL && iLen > 0 && iBase > 0 && iBase < 36)
        return _str2int(szStr, iLen-1, iBase);
    return 0;
}

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

bool _bin_init_ex(Bin *pBin, char *szNum, size_t iLen)
{
    if (pBin != NULL)
    {
        for (int i = 31, j = iLen - 1; i >= 0; i--, j--)
        {
            if (szNum != NULL && j >= 0)
                pBin->cBin[i] = szNum[j];
            else
                pBin->cBin[i] = '0';
        }
        return true;
    }
    return false;
}

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

void _bin_copy(Bin *pBTo, Bin *pBFrom)
{
    if (pBFrom != NULL && pBTo != NULL)
    {
        for (int i = 0; i < 32; i++)
            pBTo->cBin[i] = pBFrom->cBin[i];
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

void int2bin(Bin *pBin, int iNum)
{
    if (pBin != NULL)
        _int2str(iNum, 2, pBin->cBin, 32, 31, false);
}

void _bin_get_2cmplmnt(Bin *pTo, Bin *pFrom)
{
    if (pTo != NULL && pFrom != NULL)
    {
        Bin binOne, binCmplmnt;
        _bin_init(&binOne);
        int2bin(&binOne, 1);
        _bin_init(&binCmplmnt);

        _bin_copy(&binCmplmnt, pFrom);
        _bin2cmplmnt(&binCmplmnt);
        bin_add(&binCmplmnt, &binOne, pTo);
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
    if (_bin_init_ex(&b1, szB1, iLen1) && _bin_init_ex(&b2, szB2, iLen2) && _bin_init(&ret))
    {
        bin_add(&b1, &b2, &ret);
        _bin2str(&ret, szRet, iLenRet);
    }
}

void bin_mul_ex(char *szB1, size_t iLen1, char *szB2, size_t iLen2, char *szRet, size_t iLenRet)
{
    Bin b1, b2, ret;
    if (_bin_init_ex(&b1, szB1, iLen1) && _bin_init_ex(&b2, szB2, iLen2) && _bin_init(&ret))
    {
        bin_mul(&b1, &b2, &ret);
        _bin2str(&ret, szRet, iLenRet);
    }
}

void bin_sub_ex(char *szB1, size_t iLen1, char *szB2, size_t iLen2, char *szRet, size_t iLenRet)
{
    Bin b1, b2, ret;
    if (_bin_init_ex(&b1, szB1, iLen1) && _bin_init_ex(&b2, szB2, iLen2) && _bin_init(&ret))
    {
        bin_sub(&b1, &b2, &ret);
        _bin2str(&ret, szRet, iLenRet);
    }
}

void bin_div_ex(char *szB1, size_t iLen1, char *szB2, size_t iLen2, char *szRet, size_t iLenRet)
{
    Bin b1, b2, ret;
    if (_bin_init_ex(&b1, szB1, iLen1) && _bin_init_ex(&b2, szB2, iLen2) && _bin_init(&ret))
    {
        bin_div(&b1, &b2, &ret);
        _bin2str(&ret, szRet, iLenRet);
    }
}
