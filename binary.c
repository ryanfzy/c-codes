#include <string.h>
#include "binary.h"

static Bin32 BIN32S[11] =
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

void bin32_copy(Bin32 *pBTo, Bin32 *pBFrom)
{
    if (pBFrom != NULL && pBTo != NULL)
        *pBTo = *pBFrom;
}

void _bin_copy(Bin *pBTo, Bin *pBFrom)
{
    if (pBFrom != NULL && pBTo != NULL)
    {
        for (int i = 0; i < BIN_LEN; i++)
            pBTo->cBin[i] = pBFrom->cBin[i];
    }
}

bool bin32_init(Bin32 *pBin)
{
    if (pBin != NULL)
    {
        *pBin = BIN32S[0];
        return true;
    }
    return false;
}

bool _bin_init(Bin *pBin)
{
    if (pBin != NULL)
    {
        _bin_copy(pBin, &(BINS[0]));
        return true;
    }
    return false;
}

void _bin_print(Bin *pBin)
{
    if (pBin != NULL)
    {
        char szBin[33] = {0};
        memcpy(szBin, pBin->cBin, BIN_LEN);
        printf(szBin);
    }
}

bool _ch2bin(Bin *pBin, char ch)
{
    if (pBin != NULL)
    {
        int iBin = ch - '0';
        if (iBin >= 0 && iBin < 10)
        {
            _bin_copy(pBin, &BINS[iBin]);
            return true;
        }
    }
    return false;
}

void bin_lshift(Bin *pBin, int iShift)
{
    if (pBin != NULL)
    {
        for (int i = 0, j = iShift; i < BIN_LEN; i++, j++)
            pBin->cBin[i] = j < BIN_LEN ? pBin->cBin[j] : '0';
    }
}

void bin32_lshift(Bin32 *pBin, int iShift)
{
    if (pBin != NULL)
    {
        int iLshift = iShift % CHAR_BITS;
        int iRshift = CHAR_BITS - iLshift;
        for (int i = 0, j = (iShift / CHAR_BITS); i < CHAR_NUM; i++, j++)
        {
            if (BIN_LEN - i * CHAR_BITS > iShift)
            {
                unsigned char cShifted = pBin->cBin[j] >> iRshift;
                pBin->cBin[i] = pBin->cBin[j] << iLshift;
                if (i > 0)
                    pBin->cBin[i-1] |= cShifted;
            }
            else
                pBin->cBin[i] &= 0x00;
        }
    }
}

bool _bin_is_larger(Bin *pB1, Bin *pB2)
{
    if (pB1 != NULL && pB2 != NULL)
    {
        for (int i = 0; i < BIN_LEN; i++)
        {
            if (pB1->cBin[i] > pB2->cBin[i])
                return true;
            else if (pB1->cBin[i] < pB2->cBin[i])
                return false;
        }
    }
    return false;
}

bool bin_init_bstr(Bin *pBin, char *szBin, size_t iLen)
{
    if (pBin != NULL && szBin != NULL && iLen > 0)
    {
        for (int i = 31, j = iLen - 1; i >= 0; i--, j--)
            pBin->cBin[i] = j >= 0 ? szBin[j] : '0';
        return true;
    }
    return false;
}

bool bin_init_istr(Bin *pBin, char *szStr, size_t iLen)
{
    if (pBin != NULL && szStr != NULL && iLen > 0)
    {
        _bin_init(pBin);
        for (int i = 0; i < iLen; i++)
        {
            Bin curBin;
            if (_ch2bin(&curBin, szStr[i]))
            {
                if (i > 0)
                    bin_mul(pBin, &BINS[10], pBin);
                bin_add(pBin, &curBin, pBin);
            }
        }
        return true;
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
        _bin_copy(&binDen, &(BINS[1]));
        for (int i = 0; i < iLen; i++)
            bin_mul(&binDen, &(BINS[10]), &binDen);

        for (int i = 0; i < BIN_LEN; i++)
        {
            bin_lshift(&binNum, 1);
            if (bin_eq(&binNum, &binDen))
            {
                pBin->cBin[i] = '1';
                return;
            }
            else if (_bin_is_larger(&binNum, &binDen))
            {
                bin_sub(&binNum, &binDen, &binNum);
                pBin->cBin[i] = '1';
            }
        }
    }
}

void bin32_rshift(Bin32 *pBin, int iShift)
{
    if (pBin != NULL)
    {
        int iRshift = iShift % CHAR_BITS;
        int iLshift = CHAR_BITS - iRshift;
        int iLastIndex = CHAR_NUM - 1;
        for (int i = iLastIndex, j = (iLastIndex - iShift / CHAR_BITS); i >= 0; i--, j--)
        {
            if ((i+1) * CHAR_BITS > iShift)
            {
                unsigned char cShifted = pBin->cBin[j] << iLshift;
                pBin->cBin[i] = pBin->cBin[j] >> iRshift;
                if (i < iLastIndex)
                    pBin->cBin[i+1] |= cShifted;
            }
            else
                pBin->cBin[i] &= 0x00;
        }
    }
}

void bin_rshift(Bin *pBin, int iShift)
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

int _bin_get1pos(Bin *pBin)
{
    if (pBin != NULL)
    {
        for (int i = 0; i < BIN_LEN; i++)
        {
            if (pBin->cBin[i] == '1')
                return i;
        }
    }
    return -1;
}

void _fstr_get_mantissa(Bin *pB1, Bin *pB2, Bin *pBRet, int *piExp)
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

void _fstr_add_parts(Bin *pB1, Bin *pB2, Bin *pBRet)
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

        Bin binExp;
        _bin_init(&binExp);
        for (int i = 0; i < iExp; i++)
            bin_add(&binExp, &(BINS[1]), &binExp);
        bin_lshift(&binExp, 23);
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

void bin32_add(Bin32 *pB1, Bin32 *pB2, Bin32 *pBRet)
{
    if (pB1 != NULL && pB2 != NULL && pBRet != NULL)
    {
        char chCarry = 0x00;
        for (int i = CHAR_NUM - 1; i >= 0; i--)
        {
            for (int j = 0; j < 8; j++)
            {
                char chCur = 0x00;
                int iMsk = 1 << j;
                if ((pB1->cBin[i] & iMsk) == (pB2->cBin[i] & iMsk))
                {
                    chCur = chCarry;
                    chCarry = (pB1->cBin[i] & iMsk) ? 0x01 : 0x00;
                }
                else
                    chCur = (chCarry == 0x01) ? 0x00 : 0x01;
                pBRet->cBin[i] |= (chCur << j);
            }
        }
    }
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
        for (int i = 31; i >= 0; i--)
        {
            if (pB2->cBin[i] == '1')
                bin_add(pBRet, pB1, pBRet);
            bin_lshift(pB1, 1);
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
        for (int i = 0; i < BIN_LEN; i++)
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
        int iIndex = BIN_LEN-1;
        while (!bin_eq(pB1, pB2))
        {
            if (pB1->cBin[31] == '1')
            {
                bin_sub(pB1, pB2, pB1);
                pBRet->cBin[iIndex] = '1';
            }
            else
            {
                iIndex = iIndex-1;
                bin_rshift(pB1, 1);
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
        for (int i = 0; i < BIN_LEN; i++)
            pBin->cBin[i] = pBin->cBin[i] == '1' ? '0' : '1';
    }
}

void _bin_get_2cmplmnt(Bin *pTo, Bin *pFrom)
{
    if (pTo != NULL && pFrom != NULL)
    {
        _bin2cmplmnt(pFrom);
        bin_add(pFrom, &BINS[1], pTo);
    }
}

void bin32_sub(Bin32 *pB1, Bin32 *pB2, Bin32 *pBRet)
{
    if (pB1 != NULL && pB2 != NULL && pBRet != NULL)
    {
        Bin32 binCmplmnt;
        Bin32 bin2Cmplmnt = {{0x00,0x00,0x00,0x00}};
        for (int i = 0; i < CHAR_NUM; i++)
            binCmplmnt.cBin[i] = ~(pB2->cBin[i]);
        bin32_add(&binCmplmnt, &(BIN32S[1]), &bin2Cmplmnt);
        bin32_add(pB1, &bin2Cmplmnt, pBRet);
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

void _bin32_reverse_str(char *szStr, size_t iLen)
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

void bin322bstr(Bin32 *pBin, char *szStr, size_t iLen)
{
    if (pBin != NULL && szStr != NULL && iLen > 0)
    {
        for (int i = CHAR_NUM-1, j = 0; i >= 0; i--, j+=8)
        {
            for (int k = 0; k < 8; k++)
            {
                int iIndex = j + k;
                if (iIndex < iLen)
                    szStr[iIndex] = (pBin->cBin[i] & (1 << k)) ? '1' : '0';
            }
        }
    }
    _bin32_reverse_str(szStr, iLen < BIN_LEN ? iLen : BIN_LEN);
}

void bin2bstr(Bin *pBin, char *szStr, size_t iSize)
{
    if (pBin != NULL && szStr != NULL && iSize > 31)
    {
        for (int i = 0; i < BIN_LEN; i++)
            szStr[i] = pBin->cBin[i];
    }
}

void _bin2str(Bin *pBin, char *szStr, size_t iSize)
{
    if (pBin != NULL && szStr != NULL && iSize > 0)
    {
        bool bStartCpy = false;
        int iDigitCopyed = 0;
        for (int i = 0; i < BIN_LEN; i++)
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

