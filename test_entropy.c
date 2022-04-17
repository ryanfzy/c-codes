#include <stdlib.h>
#include <stdio.h>
#include "entropy.h"

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        printf("Calculate file entropy for %s\n", argv[1]);
        FILE *fp = fopen(argv[1], "r");
        if (fp != NULL)
        {
            fseek(fp, 0, SEEK_END);
            long size = ftell(fp);
            fseek(fp, 0, SEEK_SET);
            printf("Size of %s: %ld bytes\n", argv[1], size);
            char *buffer = (char*)malloc(sizeof(char)*size+1);
            fread(buffer, sizeof(char), size, fp);
            buffer[size] = '\0';
            double ent = entropy(buffer, size);
            printf("File entropy of %s: %f\n", argv[1], ent);
            free(buffer);
            fclose(fp);
        }
    }
    return 0;
}
