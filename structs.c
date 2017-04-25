#include <stdlib.h>
#include <string.h>
#include "structs.h"

////////////////////////////////////////////////////
// simple node operations
////////////////////////////////////////////////////
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

////////////////////////////////////////////////////
// named node operations
////////////////////////////////////////////////////
NamedNode* namedNode_create(char *szName, char *pData, size_t iSize)
{
    NamedNode *pNode = malloc(sizeof(NamedNode));
    namedNode_init(pNode, szName, pData, iSize);
    return pNode;
}

void namedNode_init(NamedNode *pNode, char *szName, char *pData, size_t iSize)
{
    pNode->szName = szName;
    pNode->pData = malloc(sizeof(iSize));
    pNode->iSize = iSize;
    memcpy(pNode->pData, pData, pNode->iSize);
}

void namedNode_free(NamedNode *pNode)
{
    namedNode_destroy(pNode);
    free(pNode);
}

void namedNode_destroy(NamedNode *pNode)
{
    free(pNode->pData);
    pNode->iSize = 0;
}
