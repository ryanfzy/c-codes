#ifndef NUMBER_H
#define NUMBER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void int2str(int iNum, int iBase, char *szStr, size_t iLen);
int str2int(char *szStr, size_t iLen, int iBase);

#endif
