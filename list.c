#include <stdlib.h>
#include <string.h>
#include "list.h"

#define slistnode_create unidnode_create
#define slistnode_free unidnode_free

SList* slist_create()
{
    SList *plist = malloc(sizeof(SList));
    slist_init(plist);
    return plist;
}

void slist_init(SList *plist)
{
    plist->pfirst = NULL;
    plist->plast = NULL;
    plist->icount = 0;
}

void slist_free(SList *plist)
{
    slist_destroy(plist);
    free(plist);
}

void slist_destroy(SList *plist)
{
    ListNode *pcur = plist->pfirst;
    while (pcur != NULL)
    {
        ListNode *pnext = pcur->pnext;
        slistnode_free(pcur);
        pcur = pnext;
    }
}

void slist_push(SList *plist, char *pdata, size_t isize)
{
    ListNode *pnew = slistnode_create(pdata, isize);
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

void slist_insert(SList *plist, unsigned int ipos, char *pdata, size_t isize)
{
    ipos = ipos > plist->icount ? plist->icount : ipos;
    if (ipos == plist->icount)
        slist_push(plist, pdata, isize);
    else
    {
        ListNode *pnew = slistnode_create(pdata, isize);
        ListNode *pbefore = plist->pfirst;
        for (int i = 0; i < ipos-1; i++)
            pbefore = pbefore->pnext;
        ListNode *pafter = pbefore->pnext;
        pbefore->pnext = pnew;
        pnew->pnext = pafter;
        plist->icount++;
    }
}

bool slist_remove(SList *plist, unsigned int ipos, char *pdata, size_t isize)
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
        slistnode_free(pcur);
        pbefore->pnext = pafter;
        plist->icount--;
    }
    return true;
}

void slist_remove2(SList *plist, unsigned int ipos)
{
    slist_remove(plist, ipos, NULL, 0);
}
