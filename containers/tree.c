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
            // don't use free, list_free will free the tree node
            tree_destroy(tree_get_child(ptree, i));
        list_free(ptree->pchildren);
        ptree->pchildren = NULL;
    }
    node_free(ptree->pnode);
    ptree->pnode = NULL;
}

bool tree_has_children(Tree *ptree)
{
    return tree_children_count(ptree) > 0;
}

int tree_children_count(Tree *ptree)
{
    if (ptree == NULL || ptree->pchildren == NULL)
        return 0;
    else
        return ptree->pchildren->count;
}

Tree* tree_get_child(Tree *ptree, unsigned int ichild)
{
    if (ptree == NULL || ptree->pchildren == NULL || ptree->pchildren->count == 0)
        return NULL;
    else
        return (Tree*)list_get(ptree->pchildren, ichild);
}

Tree* tree_add_child(Tree *ptree, char *pdata, size_t isize)
{
    Tree child;
    tree_init(&child, pdata, isize);
    child.parent = ptree;
    if (ptree->pchildren == NULL)
        ptree->pchildren = list_create();
    list_add(ptree->pchildren, (char*)&child, sizeof(Tree));
    return tree_get_child(ptree, ptree->pchildren->count - 1);
}

char* tree_get_data(Tree *ptree)
{
    if (ptree == NULL || ptree->pnode == NULL)
        return NULL;
    else
        return ptree->pnode->pdata;
}

/*
void tree_remove_child(Tree *parent, Tree *pchild)
{
    if (parent != NULL && pchild != NULL)
    {
        if (tree_has_children(parent))
        {
            for (int i = 0; i < tree_children_count(parent); i++)
            {
                if (tree_get_child(parent, i) == pchild)
                {
                    tree_destroy(pchild);
                    list_remove(parent->pchildren, i);
                    return;
                }
                else if (tree_has_children(tree_get_child(parent, i)))
                    tree_remove_child(tree_get_child(parent, i), pchild);
            }
        }
    }
}
*/