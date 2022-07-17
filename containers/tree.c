#include <stdlib.h>
#include <string.h>
#include "tree.h"

Tree* tree_create(char *data, size_t size)
{
    Tree *tree = malloc(sizeof(Tree));
    if (tree != NULL)
        tree_init(tree, data, size);
    return tree;
}

void tree_init(Tree *tree, char *data, size_t size)
{
    if (tree != NULL && data != NULL && size > 0)
    {
        tree->parent = NULL;
        list_init(&tree->children);
        tree->node = node_create(data, size);
    }
}

void tree_free(Tree *tree)
{
    if (tree != NULL)
    {
        tree_destroy(tree);
        free(tree);
    }
}

void tree_destroy(Tree *tree)
{
    if (tree != NULL)
    {
        if (tree_has_children(tree))
        {
            int count = tree_children_count(tree);
            for (int i = 0; i < count; i++)
                // don't use free, list_free will free the tree node
                tree_destroy(tree_get_child(tree, i));
            list_destroy(&tree->children);
        }
        node_free(tree->node);
        tree->node = NULL;
    }
}

bool tree_has_children(Tree *tree)
{
    if (tree != NULL)
        return tree_children_count(tree) > 0;
    return false;
}

int tree_children_count(Tree *tree)
{
    if (tree != NULL)
        return list_count(&tree->children);
    return 0;
}

Tree* tree_get_child(Tree *tree, unsigned int index)
{
    if (tree != NULL)
        return (Tree*)list_get(&tree->children, index);
    return NULL;
}

Tree* tree_add_child(Tree *tree, char *data, size_t size)
{
    if (tree != NULL && data != NULL && size > 0)
    {
        Tree child;
        tree_init(&child, data, size);
        child.parent = tree;
        return (Tree*)list_add(&tree->children, (char*)&child, sizeof(Tree));
    }
    return NULL;
}

char* tree_get_data(Tree *tree)
{
    if (tree != NULL && tree->node != NULL)
        return tree->node->pdata;
    return NULL;
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