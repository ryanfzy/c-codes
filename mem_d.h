#ifndef MEM_D_H
#define MEM_D_H

#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void* malloc_d(size_t size);
void free_d(void *ptr);

bool check_mem();

#ifdef __cplusplus
}
#endif

#endif
