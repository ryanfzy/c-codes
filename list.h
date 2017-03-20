#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include "structs.h"

#define ListNode UnidNode

typedef struct _list
{
    ListNode *pfirst;
    ListNode *plast;
    int icount;
} List;

List* list_create();
void list_free(List *plist);
void list_init(List *plist);
void list_destroy(List *plist);

void list_push(List *plist, char *pdata, size_t isize);
//bool list_pop(List *plist, char *pdata, size_t isize);
//void list_pop2(List *plist);
void list_insert(List *plist, unsigned int ipos, char *pdata, size_t isize);
bool list_remove(List *plist, unsigned int ipos, char *pdata, size_t isize);
void list_remove2(List *plist, unsigned int ipos);

// user can just check List struct to find out first and last
//char* list_first(List *plist);
//char* list_last(List *plist);

#endif
