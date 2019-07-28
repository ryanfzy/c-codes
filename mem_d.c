#include <stdio.h>
#include <iostream>
#include "mem_d.h"
#include <vector>
#include <algorithm>
#include <string>

static std::vector<long> vMem;
static std::vector<std::string> vMemData;

void* malloc_d(size_t size, char *pfile, int fnum)
{
    void *ptr = malloc(size);
    vMem.push_back((long)ptr);
    std::string s;
    s.append(pfile);
    s.append(",");
    s += std::to_string(fnum);
    vMemData.push_back(s);
    return ptr;
}

void free_d(void *ptr)
{
    if (ptr != NULL)
    {
        auto it_elem = std::find(vMem.begin(), vMem.end(), (long)ptr);
        int index = it_elem - vMem.begin();
        if (it_elem != vMem.end())
        {
            vMem.erase(it_elem);
            vMemData.erase(vMemData.begin() + index);
        }
    }
}

bool check_mem()
{
    if (vMem.size() == 0)
        return true;
    for (auto it_mem = vMemData.begin(); it_mem != vMemData.end(); it_mem++)
    {
        std::cout << *it_mem << std::endl;
    }
    return false;
}
