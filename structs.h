#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>

typedef struct _node
{
    char *pdata;
    size_t isize;
} Node;

typedef struct _namedNode
{
    char *szName;
    char *pData;
    size_t iSize;
} NamedNode;

// unidirected node
typedef struct _unidnode
{
	char *pdata;
    size_t isize;
	struct _unidnode *pnext;
} UnidNode;

Node* node_create(char *pdata, size_t isize);
void node_free(Node *pnode);

UnidNode* unidnode_create(char *pdata, size_t isize);
void unidnode_free(UnidNode *pnode);

NamedNode* namedNode_create(char *szName, char *pData, size_t iSize);
void namedNode_init(NamedNode *pNode, char *szName, char *pData, size_t iSize);
void namednode_free(NamedNode *pnode);

#endif
