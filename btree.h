#ifndef BTREE_H
#define BTREE_H

#include <stdlib.h>
#include <stdbool.h>

typedef long BTree;

BTree create_btree();
void free_btree(BTree tree);
void btree_insert(BTree tree, int key);
void btree_traverse(BTree tree);
void btree_delete(BTree tree, int key);
BTree btree_build(char *pstr, int size);

void btree2str(BTree tree, char *pStr, size_t size);

#endif
