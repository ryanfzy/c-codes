#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include "structs.h"

#define ListNode UnidNode

typedef struct _slist
{
    ListNode *pfirst;
    ListNode *plast;
    int icount;
} SList;

SList* slist_create();
void slist_free(SList *plist);
void slist_init(SList *plist);
void slist_destroy(SList *plist);

void slist_push(SList *plist, char *pdata, size_t isize);
//bool list_pop(List *plist, char *pdata, size_t isize);
//void list_pop2(List *plist);
void slist_insert(SList *plist, unsigned int ipos, char *pdata, size_t isize);
bool slist_remove_first(SList *plist, char *pdata, size_t isize);
bool slist_remove_last(SList *plist, char *pdata, size_t isize);
bool slist_remove(SList *plist, unsigned int ipos, char *pdata, size_t isize);
//void slist_remove2(SList *plist, unsigned int ipos);

// user can just check List struct to find out first and last
//char* list_first(List *plist);
//char* list_last(List *plist);

#endif
