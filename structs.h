#ifndef STRUCTS_H
#define STRCUTS_H

#include <stdio.h>

// unidirected node
typedef struct _unidnode
{
	char *pdata;
    size_t isize;
	struct _unidnode *pnext;
} UnidNode;

UnidNode* unidnode_create(char *pdata, size_t isize);
void unidnode_free(UnidNode *pnode);
#endif
