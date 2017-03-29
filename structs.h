#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>

typedef struct _node
{
    char *pdata;
    size_t isize;
} Node;

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
#endif
