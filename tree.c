#include <stdlib.h>
#include <string.h>
#include "tree.h"

Tree* tree_create(char *pdata, size_t isize)
{
    Tree *ptree = malloc(sizeof(Tree));
    tree_init(ptree, pdata, isize);
    return ptree;
}

void tree_init(Tree *ptree, char *pdata, size_t isize)
{
    ptree->parent = NULL;
    ptree->pchildren = NULL;
    ptree->pnode = node_create(pdata, isize);
}

void tree_free(Tree *ptree)
{
    tree_destroy(ptree);
    free(ptree);
}

void tree_destroy(Tree *ptree)
{
    if (tree_has_children(ptree))
    {
        int ichildren = tree_children_count(ptree);
        for (int i = 0; i < ichildren; i++)
            tree_destroy(tree_get_child(ptree, i));
        slist_free(ptree->pchildren);
    }
    node_free(ptree->pnode);
    free(ptree);
}

int tree_children_count(Tree *ptree)
{
    if (ptree == NULL || ptree->pchildren == NULL)
        return 0;
    else
        return ptree->pchildren->icount;
}

Tree* tree_get_child(Tree *ptree, unsigned int ichild)
{
    if (ptree == NULL || ptree->pchildren == NULL || ptree->pchildren->icount == 0)
        return NULL;
    else
        return (Tree*)slist_get(ptree->pchildren, ichild);
}

Tree* tree_add_child(Tree *ptree, char *pdata, size_t isize)
{
    Tree *pchild = tree_create(pdata, isize);
    if (ptree->pchildren == NULL)
        ptree->pchildren = slist_create();
    slist_push(ptree->pchildren, (char*)pchild, sizeof(Tree));
    return pchild;
}

char* tree_get_data(Tree *ptree)
{
    if (ptree == NULL || ptree->pnode == NULL)
        return NULL;
    else
        return ptree->pnode->pdata;
}
