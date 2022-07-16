#ifndef TREE_H
#define TREE_H

#include "structs.h"
#include "list.h"

#define TreeNode Node

typedef struct _tree Tree;

struct _tree
{
    Tree *parent;
    List *pchildren;
    TreeNode *pnode;
};

Tree* tree_create(char *pdata, size_t isize);
void tree_free(Tree *ptree);
void tree_init(Tree *ptree, char *pdata, size_t isize);
void tree_destroy(Tree *ptree);

Tree* tree_add_child(Tree *ptree, char *pdata, size_t isize);
bool tree_has_children(Tree *ptree);
int tree_children_count(Tree *ptree);
Tree* tree_get_child(Tree *ptree, unsigned int ichild);
char* tree_get_data(Tree *ptree);
//void tree_remove_child(Tree *parent, Tree *pchild);

#endif
