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

