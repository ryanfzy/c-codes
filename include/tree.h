#ifndef TREE_H
#define TREE_H

#include "structs.h"
#include "list.h"

#define TreeNode Node

typedef struct _Tree Tree;

typedef struct _Tree
{
    Tree *parent;
    List children;
    TreeNode *node;
} Tree;

Tree* tree_create(char *data, size_t size);
void tree_free(Tree *tree);
void tree_init(Tree *tree, char *data, size_t size);
void tree_destroy(Tree *ptree);

Tree* tree_add_child(Tree *tree, char *data, size_t size);
bool tree_has_children(Tree *tree);
int tree_children_count(Tree *tree);
Tree* tree_get_child(Tree *tree, unsigned int index);
char* tree_get_data(Tree *tree);
//void tree_remove_child(Tree *parent, Tree *pchild);

#endif
