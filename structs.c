#include <stdlib.h>
#include <string.h>
#include "structs.h"

Node* node_create(char *pdata, size_t isize)
{
    Node *pnode = malloc(sizeof(Node));
    pnode->pdata = NULL;
    pnode->isize = 0;
    if (isize > 0)
    {
        pnode->isize = isize;
        pnode->pdata = malloc(pnode->isize);
        memcpy(pnode->pdata, pdata, pnode->isize);
    }
    return pnode;
}

void node_free(Node *pnode)
{
    free(pnode->pdata);
    free(pnode);
}

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
