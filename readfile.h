#ifndef READFILE_H
#define READFILE_H

#include <stdbool.h>
#include <stdio.h>
#include <wchar.h>

#define UEOF -1

bool fgetuc(FILE *fp, wchar_t *pwch);
bool fgetuline(FILE *fp, wchar_t *pbuffer, size_t ibuffer, size_t *pilength, bool *pbmore);

#endif
