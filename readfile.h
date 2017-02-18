#ifndef READFILE_H
#define READFILE_H

#include <stdbool.h>
#include <stdio.h>
#include <wchar.h>

bool fgetuc(FILE *fp, wchar_t *pwch);

#endif
