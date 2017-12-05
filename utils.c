#include <string.h>
#include "utils.h"

int ymax(int a, int b)
{
    return a < b ? b : a;
}

int wstrsize(wchar_t *szStr)
{
    return (wcslen(szStr)+1) * sizeof(wchar_t) / sizeof(char);
}
