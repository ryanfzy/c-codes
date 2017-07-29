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

int _bin_cmp(Bin *pB1, Bin *pB2)
{
    if (pB1 == NULL && pB2 == NULL)
        return 0;
    else if (pB1 != NULL && pB2 == NULL)
        return 1;
    else if (pB1 == NULL && pB2 != NULL)
        return -1;
    else if (pB1 != NULL && pB2 != NULL)
    {
        for (int i = 0; i < 32; i++)
        {
            if (pB1->cBin[i] > pB2->cBin[i])
                return 1;
            else if (pB1->cBin[i] < pB2->cBin[i])
                return -1;
        }
    }
    return 0;
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

bool _bin_is_larger(Bin *pB1, Bin *pB2)
{
    if (pB1 != NULL && pB2 != NULL)
    {
        for (int i = 0; i < 32; i++)
        {
            if (pB1->cBin[i] > pB2->cBin[i])
                return true;
            else if (pB1->cBin[i] < pB2->cBin[i])
                return false;
        }
    }
    return false;
}

void _fstr2bin(Bin *pBin, char *szStr, size_t iLen)
{
    if (pBin != NULL && szStr != NULL && iLen > 0)
    {
        _bin_init(pBin);

        Bin binNum;
        bin_init_istr(&binNum, szStr, iLen);

        Bin binDen;
        _bin_init(&binDen);
        bin_add(&binDen, &(BINS[1]), &binDen);
        for (int i = 0; i < iLen; i++)
            bin_mul(&binDen, &(BINS[10]), &binDen);

        for (int i = 0; i < 32; i++)
        {
            _bin_lshift(&binNum, 1);
            if (bin_eq(&binNum, &binDen))
            {
                pBin->cBin[i] = '1';
                return;
            }
            else if (_bin_is_larger(&binNum, &binDen))
            {
                pBin->cBin[i] = '1';
                bin_sub(&binNum, &binDen, &binNum);
                pBin->cBin[i] = '1';
            }
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

bool _fstr_get_parts(char *szFloat, size_t iLen, Bin *pB1, Bin *pB2)
{
    if (szFloat != NULL && iLen > 0 && pB1 != NULL && pB2 != NULL)
    {
        int iPoint = 0;
        for (int i = 0; i < iLen; i++)
        {
            if (szFloat[i] == '.')
            {
                iPoint = i;
                break;
            }
        }
        bin_init_istr(pB1, szFloat, iPoint);
        _fstr2bin(pB2, szFloat+iPoint+1, iLen-iPoint-1);
        return true;
    }
    return false;
}

void _fstr_add_parts(Bin *pB1, Bin *pB2, Bin *pBRet)
{
    if (pB1 != NULL && pB2 != NULL && pBRet != NULL)
    {
        int iExp = 127;
        for (int i = 0, j = 31; i < 32; i++, j--)
        {
            if (pB1->cBin[i] == '1')
            {
                iExp = iExp + j;
                _bin_lshift(pB1, i+1);
                break;
            }
        }
        for (int i = 0; i < 32; i++)
        {
            if (pB2->cBin[i] == '1')
            {
                if (iExp == 127)
                {
                    _bin_lshift(pB2, i+1); 
                    iExp = 127-i-1;
                }
                else
                    _bin_rshift(pB2, iExp - 127);
                break;
            }
        }
        bin_add(pB1, pB2, pBRet);
        bool bMoreValue = false;
        for (int i = 23; i < 32; i++)
        {
            if (pBRet->cBin[i] == '1')
            {
                bMoreValue = true;
                break;
            }
        }
        _bin_rshift(pBRet, 9);
        if (bMoreValue)
            pBRet->cBin[31] = '1';

        Bin binExp;
        _bin_init(&binExp);
        for (int i = 0; i < iExp; i++)
            bin_add(&binExp, &(BINS[1]), &binExp);
        _bin_lshift(&binExp, 23);
        bin_add(pBRet, &binExp, pBRet);
    }
}

bool bin_init_fstr(Bin *pBin, char *szFloat, size_t iLen)
{
    if (pBin != NULL && szFloat != NULL && iLen > 0)
    {
        Bin bin1, bin2;
        if (_fstr_get_parts(szFloat, iLen, &bin1, &bin2))
        {
            _bin_init(pBin);
            _fstr_add_parts(&bin1, &bin2, pBin);
        }
    }
    return false;
}

void _bin_add(Bin *pB1, Bin *pB2, Bin *pBRet)
{
    if (pB1 != NULL && pB2 != NULL && pBRet != NULL)
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

void bin_add(Bin *pB1, Bin *pB2, Bin *pBRet)
{
    if (pB1 != NULL && pB2 != NULL && pBRet != NULL)
    {
        Bin bin1, bin2;
        _bin_copy(&bin1, pB1);
        _bin_copy(&bin2, pB2);
        _bin_init(pBRet);
        _bin_add(&bin1, &bin2, pBRet);
    }
}

void _bin_mul(Bin *pB1, Bin *pB2, Bin *pBRet)
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

void bin_mul(Bin *pB1, Bin *pB2, Bin *pBRet)
{
    if (pB1 != NULL && pB2 != NULL && pBRet != NULL)
    {
        Bin bin1, bin2;
        _bin_copy(&bin1, pB1);
        _bin_copy(&bin2, pB2);
        _bin_init(pBRet);
        _bin_mul(&bin1, &bin2, pBRet);
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

void _bin_div(Bin *pB1, Bin *pB2, Bin *pBRet)
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

void bin_div(Bin *pB1, Bin *pB2, Bin *pBRet)
{
    if (pB1 != NULL && pB2 != NULL && pBRet != NULL)
    {
        Bin bin1, bin2;
        _bin_copy(&bin1, pB1);
        _bin_copy(&bin2, pB2);
        _bin_init(pBRet);
        _bin_div(&bin1, &bin2, pBRet);
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

void _bin_sub(Bin *pB1, Bin *pB2, Bin *pBRet)
{
    // here use 2's complement method
    if (pB1 != NULL && pB2 != NULL && pBRet != NULL)
    {
        Bin bin2cmplmnt;
        _bin_get_2cmplmnt(&bin2cmplmnt, pB2);
        bin_add(pB1, &bin2cmplmnt, pBRet);
    }
}

void bin_sub(Bin *pB1, Bin *pB2, Bin *pBRet)
{
    if (pB1 != NULL && pB2 != NULL && pBRet != NULL)
    {
        Bin bin1, bin2;
        _bin_copy(&bin1, pB1);
        _bin_copy(&bin2, pB2);
        _bin_init(pBRet);
        _bin_sub(&bin1, &bin2, pBRet);
    }
}

void bin2bstr(Bin *pBin, char *szStr, size_t iSize)
{
    if (pBin != NULL && szStr != NULL && iSize > 31)
    {
        for (int i = 0; i < 32; i++)
            szStr[i] = pBin->cBin[i];
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
