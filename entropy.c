#include <stdio.h>
#include <math.h>
#include "entropy.h"

double entropy(char *pchar, size_t size)
{
    if (pchar != NULL && size > 0)
    {
        double ent = 0.0;
        // 8-bit entropy
        long pi[256] = {0};
        for (long i = 0; i < size; i++)
        {
            unsigned char offset = (unsigned char)(pchar[i]);
            //printf("%ld: %d - %ld\n", i, (int)pchar[i], offset);
            pi[offset] += 1;
        }
        for (int i = 0; i < 256; i++)
        {
            if (pi[i] > 0)
            {
                double p = (double)(pi[i]) / size;
                ent = ent + p * log2(p);
                //printf("%d: %f - %f\n", i, p, ent);
            }
        }
        return -ent;
    }
    return 0.0;
}
