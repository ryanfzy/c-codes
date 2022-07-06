#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>

typedef struct _node
{
    char *pdata;
    size_t isize;
} Node;

typedef struct _keyedNode
{
    char *pKey;
    size_t iKeySize;
    char *pData;
    size_t iDataSize;
} KeyedNode;

// unidirected node
typedef struct _unidnode
{
	char *pdata;
    size_t isize;
	struct _unidnode *pnext;
} UnidNode;

typedef struct _BidNode
{
    char *data;
    size_t size;
    struct _BidNode *next;
    struct _BidNode *prev;
} BidNode;

Node* node_create(char *pdata, size_t isize);
void node_free(Node *pnode);

UnidNode* unidnode_create(char *pdata, size_t isize);
void unidnode_free(UnidNode *pnode);

KeyedNode* keyedNode_create(char *pKey, size_t iKeySize, char *pData, size_t iDataSize);
void keyedNode_init(KeyedNode *pNode, char *pKey, size_t iKeySize, char *pData, size_t iDataSize);
void keyedNode_free(KeyedNode *pnode);
void keyedNode_destroy(KeyedNode *pnode);

BidNode* bidnode_create(char *data, size_t size);
void bidnode_free(BidNode *node);

#endif
