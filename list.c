#include <stdlib.h>
#include <string.h>
#include "list.h"

#define listnode_create unidnode_create
#define listnode_free unidnode_free

List* list_create()
{
    List *plist = malloc(sizeof(List));
    list_init(plist);
    return plist;
}

void list_init(List *plist)
{
    plist->pfirst = NULL;
    plist->plast = NULL;
    plist->icount = 0;
}

void list_free(List *plist)
{
    list_destroy(plist);
    free(plist);
}

void list_destroy(List *plist)
{
    ListNode *pcur = plist->pfirst;
    while (pcur != NULL)
    {
        ListNode *pnext = pcur->pnext;
        listnode_free(pcur);
        pcur = pnext;
    }
}

void list_push(List *plist, char *pdata, size_t isize)
{
    ListNode *pnew = listnode_create(pdata, isize);
    if (plist->pfirst == NULL)
    {
        plist->pfirst = pnew;
        plist->plast = pnew;
    }
    else
    {
        plist->plast->pnext = pnew;
        plist->plast = pnew;
    }
    plist->icount++;
}

void list_insert(List *plist, unsigned int ipos, char *pdata, size_t isize)
{
    ipos = ipos > plist->icount ? plist->icount : ipos;
    if (ipos == plist->icount)
        list_push(plist, pdata, isize);
    else
    {
        ListNode *pnew = listnode_create(pdata, isize);
        ListNode *pbefore = plist->pfirst;
        for (int i = 0; i < ipos-1; i++)
            pbefore = pbefore->pnext;
        ListNode *pafter = pbefore->pnext;
        pbefore->pnext = pnew;
        pnew->pnext = pafter;
        plist->icount++;
    }
}

bool list_remove(List *plist, unsigned int ipos, char *pdata, size_t isize)
{
    if (ipos >= plist->icount)
        return false;
    else
    {
        ListNode *pbefore = plist->pfirst;
        for (int i = 0; i < ipos-1; i++)
            pbefore = pbefore->pnext;
        ListNode *pcur = pbefore->pnext;
        ListNode *pafter = pcur->pnext;
        if (pdata != NULL && isize > 0)
        {
            int iminsize = isize > pcur->isize ? pcur->isize : isize;
            memcpy(pdata, pcur->pdata, iminsize);
        }
        listnode_free(pcur);
        pbefore->pnext = pafter;
        plist->icount--;
    }
    return true;
}

void list_remove2(List *plist, unsigned int ipos)
{
    list_remove(plist, ipos, NULL, 0);
}
