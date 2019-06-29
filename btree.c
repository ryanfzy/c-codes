#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "btree.h"
#include "mem_d.h"

// limitations:
// 1. key cannot be 0

#define _MALLOC malloc_d
#define _FREE free_d

//#define DEGREE 3
//#define ORDER (2*DEGREE)
#define ORDER 6
#define NODE_LEN ORDER
#define KEY_LEN (NODE_LEN-1)

typedef struct __btree_node
{
    int keys[KEY_LEN];
    struct __btree_node *pChildNodes[NODE_LEN];
} _btree_node;

_btree_node* _create_btree();
void _destroy_btree(_btree_node*);
void init_btree(_btree_node*);
bool _is_leaf(_btree_node*);
bool _is_node_full(_btree_node*);
_btree_node* _find_child_node(_btree_node*, int);
void _split_node(_btree_node*, _btree_node*);
void _insert_key_to_node(_btree_node*, int);
void _insert_key_to_tree(_btree_node*, int);
void _insert_node(_btree_node*, int, _btree_node*);
void _btree_traverse(_btree_node*);
void _print_btree(_btree_node*);
void _free_node(_btree_node*);
void _delete_key_from_node(_btree_node*, int);

BTree create_btree()
{
    _btree_node *ptree = _create_btree();
    return (BTree)ptree;
}

void free_btree(BTree tree)
{
    _btree_node *ptree = (_btree_node*)tree;
    if (ptree != NULL)
        _free_node(ptree);
}

void btree_insert(BTree tree, int key)
{
    _btree_node *ptree = (_btree_node*)tree;
    if (ptree != NULL)
        _insert_key_to_tree(ptree, key);
}

void btree_delete(BTree tree, int key)
{
    _btree_node *ptree = (_btree_node*)tree;
    if (ptree != NULL)
        _delete_key_from_node(ptree, key);
}

void btree_traverse(BTree tree)
{
    _btree_node *ptree = (_btree_node*)tree;
    if (ptree != NULL)
    {
        _print_btree(ptree);
    }
}

BTree btree_build(char *pstr, int size)
{
    if (pstr != NULL && size > 0)
    {
        int level = -1;
        _btree_node *ptrees[10]; // supports 10 level nested tree
        int indexes[level];
        char pKey[10];
        int keyIndex = 0;
        for (int i = 0; i < size; i++)
        {
            if (pstr[i] == '(')
            {
                if (keyIndex > 0)
                {
                    int key = atoi(pKey);
                    pKey[0] = '\0';
                    keyIndex = 0;
                    ptrees[level]->keys[indexes[level]] = key;
                    indexes[level]++;
                }
                level++;
                ptrees[level] = _create_btree();
                indexes[level] = 0;
            }
            else if (pstr[i] == ')')
            {
                if (keyIndex > 0)
                {
                    int key = atoi(pKey);
                    pKey[0] = '\0';
                    keyIndex = 0;
                    ptrees[level]->keys[indexes[level]] = key;
                    indexes[level]++;
                }
                if (level > 0)
                {
                    _btree_node *pnode = ptrees[level];
                    level--;
                    ptrees[level]->pChildNodes[indexes[level]] = pnode;
                }
            }
            else if (pstr[i] == ',')
            {
                int key = atoi(pKey);
                pKey[0] = '\0';
                keyIndex = 0;
                ptrees[level]->keys[indexes[level]] = key;
                indexes[level]++;
            }
            else if (pstr[i] >= '0' && pstr[i] <= '9')
            {
                pKey[keyIndex] = pstr[i];
                keyIndex++;
                pKey[keyIndex] = '\0';
            }
        }
        return (long)ptrees[0];
    }
    return 0;
}

void _free_node(_btree_node *pnode)
{
    if (pnode != NULL)
    {
        _destroy_btree(pnode);
        _FREE(pnode);
    }
}

void _btree2str(_btree_node *ptree, char **ppstr, size_t *psize)
{
    if (ptree != NULL && ppstr != NULL && *ppstr != NULL && psize != NULL && *psize > 0)
    {
        int n = sprintf(*ppstr, "(");
        *ppstr += n;
        *psize -= n;
        int i = 0;
        for (; i < KEY_LEN; i++)
        {
            _btree2str(ptree->pChildNodes[i], ppstr, psize);
            if (ptree->keys[i] != 0)
            {
                if (i > 0 && ptree->pChildNodes[i] == NULL)
                {
                    n = sprintf(*ppstr, ",");
                    *ppstr += n;
                    *psize -= n;
                }
                n = sprintf(*ppstr, "%d", ptree->keys[i]);
                *ppstr += n;
                *psize -= n;
            }
            else
            {
                i += 1;
                break;
            }
        }
        _btree2str(ptree->pChildNodes[i], ppstr, psize);
        n = sprintf(*ppstr, ")");
        *ppstr += n;
        *psize -= n;
    }
}

void btree2str(BTree tree, char *pstr, size_t size)
{
    _btree_node *ptree = (_btree_node*)tree;
    if (ptree != NULL && pstr != NULL && size > 0)
        _btree2str(ptree, &pstr, &size);
}

void _btree_traverse(_btree_node *ptree)
{
    if (ptree != NULL)
    {
        printf("(");
        int i = 0;
        for (; i < KEY_LEN; i++)
        {
            _btree_traverse(ptree->pChildNodes[i]);
            if (ptree->keys[i] != 0)
                printf("%d ", ptree->keys[i]);
            else
            {
                i += 1;
                break;
            }
        }
        _btree_traverse(ptree->pChildNodes[i]);
        printf(")");
    }
}

void _print_btree(_btree_node *pnode)
{
    if (pnode != NULL)
    {
        _btree_traverse(pnode);
        printf("\n");
    }
}

_btree_node* _create_btree()
{
    _btree_node *ptree = _MALLOC(sizeof(_btree_node));
    init_btree(ptree);
    return ptree;
}

void init_btree(_btree_node *ptree)
{
    for (int i = 0; i < KEY_LEN; i++)
        ptree->keys[i] = 0;
    for (int i = 0; i < NODE_LEN; i++)
        ptree->pChildNodes[i] = NULL;
}

void _destroy_btree(_btree_node *ptree)
{
    if (ptree != NULL)
    {
        for (int i = 0; i < NODE_LEN; i++)
        {
            _destroy_btree(ptree->pChildNodes[i]);
            _FREE(ptree->pChildNodes[i]); 
            ptree->pChildNodes[i] = NULL;
        }
    }
}

void _insert_key_to_tree(_btree_node *ptree, int key)
{
    if (ptree != NULL)
    {
        _btree_node *px = ptree;
        // split root if it is full
        if (_is_node_full(px))
            _split_node(px, px);
        // we can use recursion to replace the while loop
        // but it would be slightly slower
        do
        {
            //printf("insert %d\n", key);
            _btree_node *py = _find_child_node(px, key);
            //printf("find node ");
            //_print_btree(py);
            if (_is_node_full(py))
            {
                //_print_btree(ptree);
                _split_node(px, py);
                
                //_print_btree(ptree);
                // _find_child tries to find key from begining
                // can be optimised by _split_node returning left node or right node
                py = _find_child_node(px, key);
                //printf("after split ");
                //_print_btree(py);
            }
            px = py;
        } while (!_is_leaf(px));
        //printf("found node ");
        //_print_btree(px);
        _insert_key_to_node(px, key);
    }
}

// can be optimised
bool _is_leaf(_btree_node *pnode)
{
    if (pnode != NULL)
    {
        for (int i = 0; i < NODE_LEN; i++)
        {
            if (pnode->pChildNodes[i] != NULL)
                return false;
        }
        return true;
    }
    return false;
}

_btree_node* _find_child_node(_btree_node *pnode, int key)
{
    _btree_node *pchild = NULL;
    if (pnode != NULL)
    {
        int i = 0;
        if (key < pnode->keys[i])
        {
            pchild = pnode->pChildNodes[i];
        }
        else
        {
            for (; i < KEY_LEN; i++)
            {
                // TODO: key 0 means empty
                if (pnode->keys[i] > 0 && key > pnode->keys[i])
                {
                    //printf("%d:%d,%d\n", i, pnode->keys[i], key);
                    pchild = pnode->pChildNodes[i+1];
                }
                else
                    break;
            }
        }
        if (pchild == NULL)
            pchild = pnode;
    }
    return pchild;
}

bool _is_node_full(_btree_node *pnode)
{
    if (pnode != NULL)
    {
        for (int i = 0; i < KEY_LEN; i++)
        {
            if (pnode->keys[i] == 0)
                return false;
        }
        return true;
    }
    return false;
}

_btree_node* _split_node_by_range(_btree_node *pnode, int from, int to)
{
    _btree_node *pnode2 = NULL;
    if (pnode != NULL)
    {
        pnode2 = _create_btree();
        int j = 0;
        for (int i = from; i < to; i++)
        {
            pnode2->keys[j] = pnode->keys[i];
            pnode2->pChildNodes[j] = pnode->pChildNodes[i];
            pnode->keys[i] = 0;
            pnode->pChildNodes[i] = NULL;
            j++;
        }
        pnode2->pChildNodes[j] = pnode->pChildNodes[to];
        pnode->pChildNodes[to] = NULL;
    }
    return pnode2;
}

void _split_node(_btree_node *pParent, _btree_node *pChild)
{
    if (pParent != NULL && pChild != NULL)
    {
        //printf("start ");
        int index = ceil(KEY_LEN / 2.0)-1;
        int key = pChild->keys[index];
        pChild->keys[index] = 0;
        _btree_node *pRight = _split_node_by_range(pChild, index+1, NODE_LEN-1);
        if (pParent == pChild)
        {
            pChild = _split_node_by_range(pChild, 0, index);
            _destroy_btree(pParent);
            pParent->pChildNodes[0] = pChild;
            //printf("parent ");
            //_print_btree(pParent);
        }
        //printf("left ");
        //_print_btree(pChild);
        //printf("right ");
        //_print_btree(pRight);
        //printf("key %d\n", key);
        //printf("before parent ");
        //_print_btree(pParent);
        _insert_node(pParent, key, pRight);
        //printf("after parent ");
        //_print_btree(pParent);
    }
}

void _insert_node(_btree_node *pnode, int key, _btree_node *pRight)
{
    if (pnode != NULL)
    {
        int i = 0;
        for (; i < KEY_LEN; i++)
        {
            // TODO: key 0 means emtpy
            if (pnode->keys[i] == 0 || key < pnode->keys[i])
                break;
        }
        //printf("index %d\n", i);
        for (; i < KEY_LEN; i++)
        {
            // insert and shift right by one
            int tmp_key = pnode->keys[i];
            pnode->keys[i] = key;
            key = tmp_key;

            _btree_node *ptmp_node = pnode->pChildNodes[i+1];
            pnode->pChildNodes[i+1] = pRight;
            pRight = ptmp_node;
        }
    }
}

void _insert_key_to_node(_btree_node *pnode, int key)
{
    if (pnode != NULL)
    {
        int i = 0;
        for (; i < KEY_LEN; i++)
        {
            // TODO: key 0 means empty
            if(pnode->keys[i] == 0 || key < pnode->keys[i])
                break;
        }
        for (; i < KEY_LEN; i++)
        {
            // insert and shift right by one
            int tmp_key = pnode->keys[i];
            pnode->keys[i] = key;
            key = tmp_key;
        }
    }
}

static void _delete_key(_btree_node *pnode, int key)
{
    if (pnode != NULL)
    {
        int i = 0;
        for (; i < KEY_LEN; i++)
        {
            if (key == pnode->keys[i])
                break;
        }
        if (i < KEY_LEN)
        {
            for (; i < KEY_LEN-1; i++)
            {
                pnode->keys[i] = pnode->keys[i+1];
                // remove the right node of the key
                pnode->pChildNodes[i+1] = pnode->pChildNodes[i+2];
            }
            pnode->keys[KEY_LEN-1] = 0;
            pnode->pChildNodes[NODE_LEN-1] = NULL;
        }
    }
}

static int _find_key(_btree_node *pnode, int key)
{
    if (pnode != NULL)
    {
        for (int i = 0; i < KEY_LEN; i++)
        {
            if (pnode->keys[i] == key)
                return i;
        }
    }
    return -1;
}

static int _get_key_count(_btree_node *pnode)
{
    if (pnode != NULL)
    {
        for (int i = 0; i < KEY_LEN; i++)
        {
            if (pnode->keys[i] == 0)
                return i;
        }
        return KEY_LEN;
    }
    return 0;
}

static int _find_last_key(_btree_node *pnode)
{
    if (pnode != NULL)
    {
        for (int i = 1; i < KEY_LEN; i++)
        {
            if (pnode->keys[i] == 0)
                return pnode->keys[i-1];
        }
        return pnode->keys[KEY_LEN-1];
    }
    return 0;
}

static int _find_first_key(_btree_node *pnode)
{
    if (pnode != NULL)
    {
        return pnode->keys[0];
    }
    return 0;
}

static void _move_node(_btree_node *pTo, _btree_node *pFrom)
{
    if (pTo != NULL && pFrom != NULL)
    {
        for (int i = 0; i < KEY_LEN; i++)
        {
            pTo->keys[i] = pFrom->keys[i];
            pTo->pChildNodes[i] = pFrom->pChildNodes[i];
            pFrom->keys[i] = 0;
            pFrom->pChildNodes[i] = NULL;
        }
        pTo->pChildNodes[NODE_LEN-1] = pFrom->pChildNodes[NODE_LEN-1];
        pFrom->pChildNodes[NODE_LEN-1] = NULL;
    }
}

static void _merge_nodes(_btree_node *pParent, _btree_node *pLeft, _btree_node *pRight, int key)
{
    if (pParent != NULL && pLeft != NULL && pRight != NULL)
    {
        int i = 0;
        for (; i < KEY_LEN; i++)
        {
            if (pLeft->keys[i] == 0)
                break;
        }
        if (i < KEY_LEN)
        {
            pLeft->keys[i] = key;
            i++;
            int j = 0;
            for (; i < KEY_LEN; i++, j++)
            {
                pLeft->keys[i] = pRight->keys[j];
                pLeft->pChildNodes[i] = pRight->pChildNodes[j];
                pRight->keys[j] = 0;
                pRight->pChildNodes[j] = NULL;
            }
            pLeft->pChildNodes[NODE_LEN-1] = pRight->pChildNodes[j];
            pRight->pChildNodes[j] = NULL;
            _delete_key(pParent, key);
            // this happens if pParent is root, so it reduces the level of tree by one
            if (_get_key_count(pParent) == 0)
            {
                _move_node(pParent, pLeft);
                _free_node(pLeft);
            }
        }
    }
}

static int _find_node(_btree_node *pnode, int key)
{
    if (pnode != NULL)
    {
        for (int i = 0; i < KEY_LEN; i++)
        {
            if (pnode->keys[i] > key)
                return i;
            else if (pnode->keys[i] == key)
                return -1;
        }
        return NODE_LEN-1;
    }
    return -1;
}

void _delete_key_from_node(_btree_node *pnode, int key)
{
    int MIN_KEY_LEN = ceil(NODE_LEN / 2.0)-1;
    if (pnode != NULL)
    {
        if (_is_leaf(pnode))
        {
            _delete_key(pnode, key);
        }
        else
        {
            int i = _find_key(pnode, key);
            if (i > -1)
            {
                _btree_node *pLeft = pnode->pChildNodes[i];
                _btree_node *pRight = pnode->pChildNodes[i+1];
                if (_get_key_count(pLeft) > MIN_KEY_LEN)
                {
                    key = _find_last_key(pLeft);
                    pnode->keys[i] = key;
                    pnode = pLeft;
                }
                else if (_get_key_count(pRight) > MIN_KEY_LEN)
                {
                    key = _find_first_key(pRight);
                    pnode->keys[i] = key;
                    pnode = pRight;
                }
                else
                {
                    _merge_nodes(pnode, pLeft, pRight, key);
                    _free_node(pRight);
                }
                _delete_key_from_node(pnode, key);
            }
            else
            {
                i = _find_node(pnode, key);
                if (i > -1)
                {
                    _btree_node *pCur = pnode->pChildNodes[i];
                    if (_get_key_count(pCur) <= MIN_KEY_LEN)
                    {
                        _btree_node *pSibling = NULL;
                        int newKey = 0;
                        if (i < NODE_LEN-1)
                        {
                            pSibling = pnode->pChildNodes[i+1];
                            newKey = pnode->keys[i];
                        }
                        if (_get_key_count(pSibling) <= MIN_KEY_LEN && i > 0)
                        {
                            pSibling = pnode->pChildNodes[i-1];
                            newKey = pnode->keys[i-1];
                        }
                        if (_get_key_count(pSibling) > MIN_KEY_LEN)
                        {
                            _delete_key_from_node(pnode, newKey);
                            _insert_key_to_node(pCur, newKey);
                            _delete_key_from_node(pCur, key);
                        }
                        else
                        {
                            _btree_node *pLeft = pCur;
                            _btree_node *pRight = pSibling;
                            if (pLeft->keys[0] > pRight->keys[0])
                            {
                                pLeft = pSibling;
                                pRight = pCur;
                            }
                            _merge_nodes(pnode, pLeft, pRight, newKey);
                            _free_node(pRight);
                            _delete_key_from_node(pnode, key);
                        }
                    }
                    else
                        _delete_key_from_node(pCur, key);
                }
            }
        }
    }
}

