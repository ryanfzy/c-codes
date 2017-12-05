#include "readfile.h"

bool fgetuc(FILE *fp, wchar_t *pwch)
{
    wchar_t wch;
    char ch, mask, mask2;
    int iBytes;

    ch = fgetc(fp);
    if (ch == EOF)
        return false;
    else
    {
        mask =  0b11000000;
        mask2 = 0b01111111;
        wch = 0;
        iBytes = 0;
        while ((ch & mask) == mask)
        {
            wch <<= 6;
            wch |= fgetc(fp) & 0b00111111;
            mask >>= 1;
            mask2 >>= 1;
            iBytes++;
        }
        wch |= (ch & mask2) << (6 * iBytes);
    }
    if (pwch != NULL)
        *pwch = wch;
    return true;
}

bool fgetuline(FILE *fp, wchar_t *pbuffer, size_t ibuffer, size_t *pilength, bool *pbmore)
{
    wchar_t wch;
    
    if (pilength != NULL)
        *pilength = 0;
    if (pbmore != NULL)
        *pbmore = true;

    for (int i = 0; i < ibuffer; i++)
    {
        if (fgetuc(fp, &wch))
        {
            if (wch == 0x000d)
                fgetuc(fp, &wch);
            if (wch == 0x000a)
            {
                if (pilength != NULL)
                    *pilength = i;
                if (pbmore != NULL)
                    *pbmore = false;
                return true;
            }
            pbuffer[i] = wch;
        }
        else
            return false;
    }
    if (pilength != NULL)
        *pilength = ibuffer;
    return true;
}
