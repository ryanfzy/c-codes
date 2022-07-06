#include <stdlib.h>
#include <string.h>
#include "list.h"

#define listnode_create bidnode_create
#define listnode_free bidnode_free

List* list_create()
{
    List *list = malloc(sizeof(List));
    list_init(list);
    return list;
}

void list_init(List *list)
{
    if (list != NULL)
    {
        list->first = NULL;
        list->last = NULL;
        list->count = 0;
    }
}

void list_free(List *list)
{
    if (list != NULL)
    {
        list_destroy(list);
        free(list);
    }
}

void list_destroy(List *list)
{
    if (list != NULL)
    {
        ListNode *cur = list->first;
        while (cur != NULL)
        {
            ListNode *next = cur->next;
            listnode_free(cur);
            cur = next;
        }
        list_init(list);
    }
}

static void _list_node_link(ListNode *node1, ListNode *node2)
{
    if (node1 != NULL)
        node1->next = node2;
    if (node2 != NULL)
        node2->prev = node1;
}

bool list_add(List *list, char *data, size_t size)
{
    if (list != NULL && data != NULL && size > 0)
    {
        ListNode *node = listnode_create(data, size);
        if (list->first == NULL)
            list->first = list->last = node;
        else
        {
            _list_node_link(list->last, node);
            list->last = node;
        }
        list->count++;
        return true;
    }
    return false;
}

ListNode* _list_get(List *list, int pos)
{
    if (pos >= 0)
    {
        ListNode *node = list->first;
        while (node != NULL && pos > 0 && node != NULL)
        {
            node = node->next;
            pos--;
        }
        return node;
    }
    else
    {
        pos++;
        ListNode *node = list->last;
        while (node != NULL && pos < 0 && node != NULL)
        {
            node = node->prev;
            pos++;
        }
        return node;
    }
}

bool list_insert(List *list, int pos, char *data, size_t size)
{
    if (list != NULL && data != NULL && size > 0)
    {
        ListNode *node = listnode_create(data, size);
        if (list->count == 0)
        {
            if (pos == 0 || pos == -1)
            {
                list->first = list->last = node;
                list->count++;
                return true;
            }
        }
        else
        {
            ListNode *cur = _list_get(list, pos);
            if (cur == list->first)
                list->first = node;
            else if (cur != NULL)
                _list_node_link(cur->prev, node);
            else
                return false;
            _list_node_link(node, cur);
            list->count++;
            return true;
        }
    }
    return false;
}

bool list_remove(List *list, unsigned int pos, char *data, size_t size)
{
    if (list != NULL && data != NULL && size > 0)
    {
        pos = pos >= list->count ? list->count-1 : pos;
        ListNode *cur = _list_get(list, pos);
        _list_node_link(cur->prev, cur->next);
        int minsize = size > cur->size ? cur->size : size;
        memcpy(data, cur->data, minsize);
        listnode_free(cur);
        list->count--;
        return true;
    }
    return false;
}

char* list_get(List *list, int pos)
{
    if (list != NULL)
    {
        ListNode *cur = _list_get(list, pos);
        return cur->data;
    }
    return NULL;
}

size_t list_count(List *list)
{
    if (list != NULL)
        return list->count;
    return 0;
}
