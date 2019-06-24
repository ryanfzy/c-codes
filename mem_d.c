#include <stdio.h>
#include "mem_d.h"
#include <vector>
#include <algorithm>

static std::vector<long> vMem;

void* malloc_d(size_t size)
{
    void *ptr = malloc(size);
    vMem.push_back((long)ptr);
    //printf("malloc %lu\n", (long)ptr);
    return ptr;
}

void free_d(void *ptr)
{
    if (ptr != NULL)
    {
        auto it_elem = std::find(vMem.begin(), vMem.end(), (long)ptr);
        if (it_elem != vMem.end())
        {
            vMem.erase(it_elem);
            //printf("free %lu\n", (long)ptr);
        }
    }
}

bool check_mem()
{
    //printf("size %d\n", vMem.size());
    return vMem.size() == 0;
}
