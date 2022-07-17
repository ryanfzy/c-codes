#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include "structs.h"

#define ListNode BidNode

typedef struct _list
{
    ListNode *first;
    ListNode *last;
    int count;
} List;

List* list_create();
void list_free(List *list);
void list_init(List *list);
void list_destroy(List *list);

char* list_add(List *list, char *data, size_t size);
bool list_insert(List *list, int pos, char *data, size_t size);
bool list_remove(List *list, unsigned int pos, char *data, size_t size);
char* list_get(List *list, int pos);
size_t list_count(List *list);

#endif
