#include <stdlib.h>
#include <string.h>
#include "structs.h"

////////////////////////////////////////////////////
// simple node operations
////////////////////////////////////////////////////
Node* node_create(char *pdata, size_t isize)
{
    Node *pnode = malloc(sizeof(Node));
    if (pnode != NULL)
    {
        pnode->pdata = NULL;
        pnode->isize = 0;
        if (isize > 0)
        {
            pnode->isize = isize;
            pnode->pdata = malloc(pnode->isize);
            if (pnode->pdata != NULL)
                memcpy(pnode->pdata, pdata, pnode->isize);
        }
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
    if (pnode != NULL)
    {
        pnode->pdata = NULL;
        pnode->isize = 0;
        pnode->pnext = NULL;
        if (isize > 0)
        {
            pnode->isize = isize;
            pnode->pdata = malloc(pnode->isize);
            if (pnode->pdata != NULL)
                memcpy(pnode->pdata, pdata, pnode->isize);
        }
    }
    return pnode;
}

void unidnode_free(UnidNode *pnode)
{
    free(pnode->pdata);
    //free(pNode->pNext);  // don't free pNext
    free(pnode);
}

////////////////////////////////////////////////////
// keyed node operations
////////////////////////////////////////////////////
KeyedNode* keyedNode_create(char *pKey, size_t iKeySize, char *pData, size_t iDataSize)
{
    if (pKey != NULL && iKeySize > 0 && pData != NULL && iDataSize > 0)
    {
        KeyedNode *pNode = malloc(sizeof(KeyedNode));
        keyedNode_init(pNode, pKey, iKeySize, pData, iDataSize);
        return pNode;
    }
    return NULL;
}

void keyedNode_init(KeyedNode *pNode, char *pKey, size_t iKeySize, char *pData, size_t iDataSize)
{
    if (pNode != NULL && pKey != NULL && iKeySize > 0 && pData != NULL && iDataSize > 0)
    {
        pNode->iKeySize = iKeySize;
        pNode->pKey = malloc(pNode->iKeySize);
        memcpy(pNode->pKey, pKey, pNode->iKeySize);

        pNode->iDataSize = iDataSize;
        pNode->pData = malloc(pNode->iDataSize);
        memcpy(pNode->pData, pData, pNode->iDataSize);
    }
}

void keyedNode_free(KeyedNode *pNode)
{
    if (pNode != NULL)
    {
        keyedNode_destroy(pNode);
        free(pNode);
    }
}

void keyedNode_destroy(KeyedNode *pNode)
{
    if (pNode != NULL && pNode->pKey != NULL && pNode->pData != NULL)
    {
        free(pNode->pKey);
        pNode->iKeySize = 0;

        free(pNode->pData);
        pNode->iDataSize = 0;
    }
}


BidNode* bidnode_create(char *data, size_t size)
{
    if (data != NULL && size > 0)
    {
        BidNode *node = malloc(sizeof(BidNode));
        if (node != NULL)
        {
            node->next = NULL;
            node->prev = NULL;
            node->size = size;
            node->data = malloc(node->size);
            memcpy(node->data, data, node->size);
        }
        return node;
    }
    return NULL;
}

void bidnode_free(BidNode *node)
{
    if (node != NULL)
    {
        free(node->data);
        free(node);
    }
}