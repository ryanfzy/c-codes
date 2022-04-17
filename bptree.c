#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "bptree.h"
#include "mem_d.h"

// limitations:
// 1. key cannot be 0

#define _MALLOC malloc_d
#define _FREE free_d

//#define DEGREE 3
//#define ORDER (2*DEGREE)
#define ORDER 5
#define NODE_LEN ORDER
#define KEY_LEN (NODE_LEN-1)
int MIN_KEY_LEN = ceil(NODE_LEN / 2.0)-1;

typedef struct __bptree_node
{
    int keys[KEY_LEN];
    struct __bptree_node *pChildNodes[NODE_LEN];
    struct __bptree_node *pnext;  // only used by leaf nodes
} _bptree_node;

_bptree_node* _create_bptree();
void _destroy_bptree(_bptree_node*);
void init_bptree(_bptree_node*);
bool _is_leaf(_bptree_node*);
bool _is_node_full(_bptree_node*);
_bptree_node* _find_child_node(_bptree_node*, int);
void _split_node(_bptree_node*, _bptree_node*, int);
void _insert_key_to_leaf(_bptree_node*, int);
void _insert_key_to_node(_bptree_node*, int);
void _insert_node(_bptree_node*, int, _bptree_node*);
void _bptree_traverse(_bptree_node*);
void _print_bptree(_bptree_node*);
void _free_node(_bptree_node*);
void _delete_key_from_node(_bptree_node*, int);

BpTree create_bptree()
{
    _bptree_node *ptree = _create_bptree();
    return (BpTree)ptree;
}

void free_bptree(BpTree tree)
{
    _bptree_node *ptree = (_bptree_node*)tree;
    if (ptree != NULL)
        _free_node(ptree);
}

void bptree_insert(BpTree tree, int key)
{
    _bptree_node *ptree = (_bptree_node*)tree;
    if (ptree != NULL)
        _insert_key_to_node(ptree, key);
}

void bptree_delete(BpTree tree, int key)
{
    _bptree_node *ptree = (_bptree_node*)tree;
    if (ptree != NULL)
        _delete_key_from_node(ptree, key);
}

void bptree_traverse(BpTree tree)
{
    _bptree_node *ptree = (_bptree_node*)tree;
    if (ptree != NULL)
    {
        _print_bptree(ptree);
    }
}

BpTree bptree_build(char *pstr, int size)
{
    if (pstr != NULL && size > 0)
    {
        int level = -1;
        _bptree_node *ptrees[10]; // supports 10 level nested tree
        int indexes[level];
        char pKey[10];
        int keyIndex = 0;
        int leafLevel = 0;
        _bptree_node *pLeaf = NULL;
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
                ptrees[level] = _create_bptree();
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
                    _bptree_node *pnode = ptrees[level];
                    level--;
                    ptrees[level]->pChildNodes[indexes[level]] = pnode;

                    if (pLeaf == NULL)
                    {
                        pLeaf = pnode;
                        leafLevel = level;
                    }
                    else if(leafLevel == level)
                    {
                        pLeaf->pnext = pnode;
                        pLeaf = pnode;
                    }
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

void _free_node(_bptree_node *pnode)
{
    if (pnode != NULL)
    {
        _destroy_bptree(pnode);
        _FREE(pnode);
    }
}

void _bptree2str(_bptree_node *ptree, char **ppstr, size_t *psize)
{
    if (ptree != NULL && ppstr != NULL && *ppstr != NULL && psize != NULL && *psize > 0)
    {
        int n = sprintf(*ppstr, "(");
        *ppstr += n;
        *psize -= n;
        int i = 0;
        for (; i < KEY_LEN; i++)
        {
            _bptree2str(ptree->pChildNodes[i], ppstr, psize);
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
        if (!_is_leaf(ptree))
            _bptree2str(ptree->pChildNodes[i], ppstr, psize);
        n = sprintf(*ppstr, ")");
        *ppstr += n;
        *psize -= n;
    }
}

void bptree2str(BpTree tree, char *pstr, size_t size)
{
    _bptree_node *ptree = (_bptree_node*)tree;
    if (ptree != NULL && pstr != NULL && size > 0)
        _bptree2str(ptree, &pstr, &size);
}

void _bptree_keys2str(_bptree_node *pnode, char *pstr, size_t size)
{
    if (pnode != NULL && pstr != NULL && size > 0)
    {
        while (!_is_leaf(pnode))
            pnode = pnode->pChildNodes[0];
        while (pnode != NULL)
        {
            for (int i = 0; i < KEY_LEN; i++)
            {
                if (size < 1)
                    return;
                int key = pnode->keys[i];
                if (key > 0)
                {
                    int n = sprintf(pstr, "%d,", key);
                    pstr += n;
                    size -= n;
                }
            }
            pnode = pnode->pnext;
        }
        // remove last comma
        pstr -= 1;
        if (*pstr == ',')
            *pstr = '\0';
    }
}

void bptree_keys2str(BpTree tree, char *pstr, size_t size)
{
    _bptree_node *ptree = (_bptree_node*)tree;
    if (ptree != NULL)
        _bptree_keys2str(ptree, pstr, size);
}

void _bptree_traverse(_bptree_node *ptree)
{
    if (ptree != NULL)
    {
        printf("(");
        int i = 0;
        for (; i < KEY_LEN; i++)
        {
            _bptree_traverse(ptree->pChildNodes[i]);
            if (ptree->keys[i] != 0)
                printf("%d ", ptree->keys[i]);
            else
            {
                i += 1;
                break;
            }
        }
        if (!_is_leaf(ptree))
            _bptree_traverse(ptree->pChildNodes[i]);
        printf(")");
    }
}

void _print_bptree(_bptree_node *pnode)
{
    if (pnode != NULL)
    {
        _bptree_traverse(pnode);
        printf("\n");
    }
}

_bptree_node* _create_bptree()
{
    _bptree_node *ptree = _MALLOC(sizeof(_bptree_node));
    init_bptree(ptree);
    return ptree;
}

void init_bptree(_bptree_node *ptree)
{
    for (int i = 0; i < KEY_LEN; i++)
        ptree->keys[i] = 0;
    for (int i = 0; i < NODE_LEN; i++)
        ptree->pChildNodes[i] = NULL;
    ptree->pnext = NULL;
}

void _destroy_bptree(_bptree_node *ptree)
{
    if (ptree != NULL)
    {
        for (int i = 0; i < NODE_LEN; i++)
        {
            _destroy_bptree(ptree->pChildNodes[i]);
            _FREE(ptree->pChildNodes[i]); 
            ptree->pChildNodes[i] = NULL;
        }
        ptree->pnext = NULL;
    }
}

void _insert_key_to_node(_bptree_node *ptree, int key)
{
    if (ptree != NULL)
    {
        _bptree_node *px = ptree;
        if (_is_node_full(px))
            _split_node(px, px, key);
        do
        {
            _bptree_node *py = _find_child_node(px, key);
            if (_is_node_full(py))
            {
                _split_node(px, py, key);
                // can be optimised by _split_node returning left node or right node
                py = _find_child_node(px, key);
            }
            px = py;
        } while (!_is_leaf(px));
        _insert_key_to_leaf(px, key);
    }
}

bool _is_leaf(_bptree_node *pnode)
{
    if (pnode != NULL)
        return pnode->pChildNodes[0] == NULL;
    return false;
}

_bptree_node* _find_child_node(_bptree_node *pnode, int key)
{
    _bptree_node *pchild = NULL;
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
                if (pnode->keys[i] > 0 && key >= pnode->keys[i])
                    pchild = pnode->pChildNodes[i+1];
                else
                    break;
            }
        }
        if (pchild == NULL)
            pchild = pnode;
    }
    return pchild;
}

bool _is_node_full(_bptree_node *pnode)
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

_bptree_node* _split_node_by_range(_bptree_node *pnode, int from, int to)
{
    _bptree_node *pnode2 = NULL;
    if (pnode != NULL)
    {
        pnode2 = _create_bptree();
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
        // link nodes if they are leaf
        if (_is_leaf(pnode))
        {
            pnode2->pnext = pnode->pnext;
            pnode->pnext = pnode2;
        }
    }
    return pnode2;
}

void _split_node(_bptree_node *pParent, _bptree_node *pChild, int newKey)
{
    if (pParent != NULL && pChild != NULL)
    {
        bool isLeaf = _is_leaf(pChild);
        // the index will be off by one, but it is fine for leaf
        // because its right child will contain a duplicate key
        int index = ceil(KEY_LEN / 2.0);
        // for internal node, need to fix the off by one issue
        index = isLeaf ? index : index-1;
        int key = pChild->keys[index];
        int fromIndex = index;
        // when splits leaf node, keep the key in the right child node
        // when splits internal node, don't push the key into child node
        if (!isLeaf)
        {
            fromIndex++;
            pChild->keys[index] = 0;
        }
        _bptree_node *pRight = _split_node_by_range(pChild, fromIndex, NODE_LEN-1);
        if (pParent == pChild)
        {
            pChild = _split_node_by_range(pChild, 0, index);
            _destroy_bptree(pParent);
            pParent->pChildNodes[0] = pChild;
        }
        _insert_node(pParent, key, pRight);
    }
}

void _insert_node(_bptree_node *pnode, int key, _bptree_node *pRight)
{
    if (pnode != NULL)
    {
        // find the position to insert the key
        int i = 0;
        for (; i < KEY_LEN; i++)
        {
            // TODO: key 0 means emtpy
            if (pnode->keys[i] == 0 || key < pnode->keys[i])
                break;
        }
        // insert and shift all other keys and child nodes right by one
        for (; i < KEY_LEN; i++)
        {
            int tmp_key = pnode->keys[i];
            pnode->keys[i] = key;
            key = tmp_key;

            _bptree_node *ptmp_node = pnode->pChildNodes[i+1];
            pnode->pChildNodes[i+1] = pRight;
            pRight = ptmp_node;
        }
    }
}

void _insert_key_to_leaf(_bptree_node *pnode, int key)
{
    if (pnode != NULL)
    {
        // find the position to insert the key
        int i = 0;
        for (; i < KEY_LEN; i++)
        {
            // TODO: key 0 means empty
            if(pnode->keys[i] == 0 || key < pnode->keys[i])
                break;
        }
        // insert and shift other keys right by one
        for (; i < KEY_LEN; i++)
        {
            int tmp_key = pnode->keys[i];
            pnode->keys[i] = key;
            key = tmp_key;
        }
    }
}

static void _delete_key_from_leaf(_bptree_node *pnode, int key)
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
                pnode->keys[i] = pnode->keys[i+1];
            pnode->keys[KEY_LEN-1] = 0;
        }
    }
}

static int _find_key(_bptree_node *pnode, int key)
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

static int _get_key_count(_bptree_node *pnode)
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

static void _move_node(_bptree_node *pTo, _bptree_node *pFrom)
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
        pTo->pnext = pFrom->pnext;
    }
}

static int _find_node(_bptree_node *pnode, int key)
{
    if (pnode != NULL)
    {
        for (int i = 0; i < KEY_LEN; i++)
        {
            if (pnode->keys[i] == 0 || pnode->keys[i] > key)
                return i;
            // this should never happen for internal nodes
            else if (pnode->keys[i] == key)
                return -1;
        }
        return NODE_LEN-1;
    }
    return -1;
}

// very similar to _shift_node
static void _shift_leaf(_bptree_node *pnode, int leafIndex)
{
    if (pnode != NULL)
    {
        _bptree_node *pleft = pnode->pChildNodes[leafIndex];
        _bptree_node *pchild = pnode->pChildNodes[leafIndex+1];
        int lastKeyIndex = _get_key_count(pleft)-1;
        int lastKey = pleft->keys[lastKeyIndex];
        pleft->keys[lastKeyIndex] = 0;
        for (int i = KEY_LEN-1; i > 0; i--)
            pchild->keys[i] = pchild->keys[i-1];
        pchild->keys[0] = lastKey;
        pnode->keys[leafIndex] = lastKey;
    }
}

// very similar to _merge_node
static void _merge_leaf(_bptree_node *pnode, int leafIndex)
{
    if (pnode != NULL)
    {
        _bptree_node *pleaf = pnode->pChildNodes[leafIndex];
        _bptree_node *pright = pnode->pChildNodes[leafIndex+1];
        int i = _get_key_count(pleaf);
        for (int j = 0; i < KEY_LEN; i++, j++)
            pleaf->keys[i] = pright->keys[j];
        pleaf->pnext = pright->pnext;
        i = leafIndex;
        for (; i < KEY_LEN-1; i++)
        {
            pnode->keys[i] = pnode->keys[i+1];
            pnode->pChildNodes[i+1] = pnode->pChildNodes[i+2];
        }
        pnode->keys[KEY_LEN-1] = 0;
        pnode->pChildNodes[NODE_LEN-1] = NULL;
        _free_node(pright);
    }
}

// very similar to _shift_leaf
static void _shift_node(_bptree_node *pnode, int nodeIndex)
{
    if (pnode != NULL)
    {
        _bptree_node *pchild = pnode->pChildNodes[nodeIndex];
        _bptree_node *pleft = pnode->pChildNodes[nodeIndex-1];
        int lastChildIndex = _get_key_count(pleft);
        int lastKeyIndex = lastChildIndex-1;
        int lastKey = pleft->keys[lastKeyIndex];
        _bptree_node *pLastChild = pleft->pChildNodes[lastChildIndex];
        pleft->keys[lastKeyIndex] = 0;
        pleft->pChildNodes[lastChildIndex] = NULL;
        for (int i = KEY_LEN-1; i > 0; i--)
        {
            pchild->keys[i] = pchild->keys[i-1];
            pchild->pChildNodes[i+1] = pchild->pChildNodes[i];
        }
        pchild->pChildNodes[1] = pchild->pChildNodes[0];
        int oldKey = pnode->keys[nodeIndex-1];
        pnode->keys[nodeIndex-1] = lastKey;
        pchild->keys[0] = oldKey;
        pchild->pChildNodes[0] = pLastChild;
    }
}

// very similar to _merge_leaf
static void _merge_node(_bptree_node *pnode, int nodeIndex)
{
    if (pnode != NULL)
    {
        _bptree_node *pchild = pnode->pChildNodes[nodeIndex];
        _bptree_node *pleft = pnode->pChildNodes[nodeIndex-1];
        int startIndex = _get_key_count(pleft);
        int parentKey = pnode->keys[nodeIndex-1];
        pleft->keys[startIndex] = parentKey;
        pleft->pChildNodes[startIndex+1] = pchild->pChildNodes[0];
        pchild->pChildNodes[0] = NULL;
        startIndex++;
        for (int j = 0; startIndex < KEY_LEN; startIndex++, j++)
        {
            pleft->keys[startIndex] = pchild->keys[j];
            pleft->pChildNodes[startIndex+1] = pchild->pChildNodes[j+1];
            pchild->keys[j] = 0;
            pchild->pChildNodes[j+1] = 0;
        }
        for (int i = nodeIndex-1; i < KEY_LEN-1; i++)
        {
            pnode->keys[i] = pnode->keys[i+1];
            pnode->pChildNodes[i+1] = pnode->pChildNodes[i+2];
        }
        pnode->keys[KEY_LEN-1] = 0;
        pnode->pChildNodes[NODE_LEN-1] = NULL;
        _free_node(pchild);
        if (_get_key_count(pnode) == 0)
        {
            _move_node(pnode, pleft);
            _free_node(pleft);
        }
    }
}

static void _delete_key_from_node_with_leaf(_bptree_node *pnode, int leafIndex, int key)
{
    if (pnode != NULL)
    {
        _bptree_node *pchild = pnode->pChildNodes[leafIndex];
        _bptree_node *pleft = pnode->pChildNodes[leafIndex-1];
        if (_get_key_count(pchild) > MIN_KEY_LEN)
        {
            if (pnode->keys[leafIndex-1] == key)
                pnode->keys[leafIndex-1] = pchild->keys[1];
        }
        else if (_get_key_count(pleft) > MIN_KEY_LEN)
        {
            _shift_leaf(pnode, leafIndex-1);
        }
        else
        {
            _merge_leaf(pnode, leafIndex-1);
            pchild = pleft;
        }
        _delete_key_from_leaf(pchild, key);
    }
}

static void _delete_key_from_node_with_node(_bptree_node *pnode, int nodeIndex, int key)
{
    if (pnode != NULL)
    {
        _bptree_node *pchild = pnode->pChildNodes[nodeIndex];
        _bptree_node *pleft = pnode->pChildNodes[nodeIndex-1];
        if (_get_key_count(pchild) >= MIN_KEY_LEN)
        {
            _delete_key_from_node(pchild, key);
        }
        else if (_get_key_count(pleft) > MIN_KEY_LEN)
        {
            _shift_node(pnode, nodeIndex);
            _delete_key_from_node(pchild, key);
        }
        else
        {
            _merge_node(pnode, nodeIndex);
            _delete_key_from_node(pnode, key);
        }
    }
}

void _delete_key_from_node(_bptree_node *pnode, int key)
{
    if (pnode != NULL)
    {
        int i = _find_key(pnode, key);
        if (i >= 0)
        {
            if (_is_leaf(pnode))
                _delete_key_from_leaf(pnode, key);
            else
            {
                if (_is_leaf(pnode->pChildNodes[i+1]))
                {
                    _delete_key_from_node_with_leaf(pnode, i+1, key);
                }
                else
                {
                    _delete_key_from_node_with_node(pnode, i+1, key);
                }
            }
        }
        else if (!_is_leaf(pnode))
        {
            int i = _find_node(pnode, key);
            if (i >= 0)
            {
                if (_is_leaf(pnode->pChildNodes[i]))
                {
                    _delete_key_from_node_with_leaf(pnode, i, key);
                }
                else
                    _delete_key_from_node_with_node(pnode, i, key);
            }
        }
    }
}

