#include <stdlib.h>
#include <string.h>
#include "structs.h"

////////////////////////////////////////////////////
// unidirected node operations
////////////////////////////////////////////////////
UnidNode* unidnode_create(char *pdata, size_t isize)
{
    UnidNode *pnode = malloc(sizeof(UnidNode));
    if (isize > 0)
    {
        pnode->isize = isize;
        pnode->pdata = malloc(pnode->isize);
        memcpy(pnode->pdata, pdata, pnode->isize);
    }
    else
    {
        pnode->pdata = NULL;
        pnode->isize = 0;
    }
    pnode->pnext = NULL;
    return pnode;
}

void unidnode_free(UnidNode *pnode)
{
    free(pnode->pdata);
    //free(pNode->pNext);  // don't free pNext
    free(pnode);
}
