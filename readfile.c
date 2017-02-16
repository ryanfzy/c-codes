#include <stdio.h>
#include <wchar.h>
#include <stdbool.h>

bool fgetuc(FILE *fp, wchar_t *pwch)
{
    wchar_t wch;
    char ch, mask, mask2, chars[3];
    int iBytes;

    ch = fgetc(fp);
    if (ch == EOF)
    {
        return false;
    }
    else
    {
        mask =  0b11000000;
        mask2 = 0b00111111;
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

int main(void)
{
    FILE *fp = fopen("test2.txt", "rb");
    wchar_t wch;
    while (fgetuc(fp, &wch))
    {
        printf("%x\n", wch);
    }
    fclose(fp);
}
