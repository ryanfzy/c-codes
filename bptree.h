#ifndef BPTREE_H
#define BPTREE_H

#include <stdlib.h>

typedef long BpTree;

BpTree create_bptree();
void free_bptree(BpTree tree);
void bptree_insert(BpTree tree, int key);
void bptree_traverse(BpTree tree);
void bptree_delete(BpTree tree, int key);
BpTree bptree_build(char *pstr, int size);

void bptree2str(BpTree tree, char *pStr, size_t size);
void bptree_keys2str(BpTree tree, char *pstr, size_t size);

#endif
