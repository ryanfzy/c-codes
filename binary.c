#include <string.h>
#include "binary.h"

static Bin BINS[11] =
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

void bin_copy(Bin *pBTo, Bin *pBFrom)
{
    if (pBFrom != NULL && pBTo != NULL)
        *pBTo = *pBFrom;
}

bool bin_init(Bin *pBin)
{
    if (pBin != NULL)
    {
        *pBin = BINS[0];
        return true;
    }
    return false;
}

void bin_lshift(Bin *pBin, int iShift)
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

bool bin_init_istr(Bin *pBin, char *szStr, size_t iLen)
{
    if (pBin != NULL && szStr != NULL && iLen > 0)
    {
        bin_init(pBin);
        for (int i = 0; i < iLen; i++)
        {
            int iBin = szStr[i] - '0';
            Bin binRet = *pBin;
            bin_init(pBin);
            if (iBin >= 0 && iBin <= 9)
            {
                if (i > 0)
                {
                    bin_mul(&binRet, &(BINS[10]), pBin);
                    binRet = *pBin;
                }
                bin_add(&binRet, &(BINS[iBin]), pBin);
            }
        }
        return true;
    }
    return false;
}

void bin_rshift(Bin *pBin, int iShift)
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

void bin_add(Bin *pB1, Bin *pB2, Bin *pBRet)
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

void bin_mul(Bin *pB1, Bin *pB2, Bin *pBRet)
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
                Bin lastRet = *pBRet;
                bin_add(&lastRet, pB1, pBRet);
            }
            bin_lshift(pB1, 1);
        }
    }
}

bool bin_eq(Bin *pB1, Bin *pB2)
{
    for (int i = 0; i < CHAR_NUM; i++)
    {
        if (pB1->cBin[i] != pB2->cBin[i])
            return false;
    }
    return true;
}

void bin_div(Bin *pB1, Bin *pB2, Bin *pBRet)
{
    if (pB1 != NULL && pB2 != NULL && pBRet != NULL)
    {
        int iIndex = BIN_LEN-1;
        Bin bin1 = *pB1;
        while (!bin_eq(&bin1, &(BINS[0])))
        {
            int iBitOffset = CHAR_BITS - (iIndex % CHAR_BITS) - 1;
            if (bin1.cBin[CHAR_NUM-1] & 1)
            {
                Bin binRet = {{0x00,0x00,0x00,0x00}};
                bin_sub(&bin1, pB2, &binRet);
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

void bin_sub(Bin *pB1, Bin *pB2, Bin *pBRet)
{
    if (pB1 != NULL && pB2 != NULL && pBRet != NULL)
    {
        Bin binCmplmnt;
        Bin bin2Cmplmnt = {{0x00,0x00,0x00,0x00}};
        for (int i = 0; i < CHAR_NUM; i++)
            binCmplmnt.cBin[i] = ~(pB2->cBin[i]);
        bin_add(&binCmplmnt, &(BINS[1]), &bin2Cmplmnt);
        bin_add(pB1, &bin2Cmplmnt, pBRet);
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

void bin2bstr(Bin *pBin, char *szStr, size_t iLen)
{
    if (pBin != NULL && szStr != NULL && iLen > 0)
    {
        for (int i = BIN_LEN - 1, j = 0; i >= 0; i--, j++)
        {
            int iIndex = i / CHAR_BITS;
            int iBitOffset = CHAR_BITS - (i % CHAR_BITS) - 1;

            if (j < iLen)
                szStr[j] = (pBin->cBin[iIndex] & (1 << iBitOffset)) ? '1' : '0';
        }
    }
    _bin_reverse_str(szStr, iLen < BIN_LEN ? iLen : BIN_LEN);
}

/*
// float
void _fstr2bin(Bin *pBin, char *szStr, size_t iLen)
{
    if (pBin != NULL && szStr != NULL && iLen > 0) {
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
*/
