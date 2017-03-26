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
    plist->pfirst = NULL;
    plist->plast = NULL;
    plist->icount = 0;
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

bool slist_remove_first(SList *plist, char *pdata, size_t isize)
{
    if (plist->icount < 0)
        return false;
    else
    {
        ListNode *pfirst = plist->pfirst;
        plist->pfirst = pfirst->pnext;
        if (plist->pfirst == NULL)
            plist->plast = NULL;
        int iminsize = isize > pfirst->isize ? pfirst->isize : isize;
        memcpy(pdata, pfirst->pdata, iminsize);
        slistnode_free(pfirst);
        plist->icount--;
        return true;
    }
}

bool slist_remove_last(SList *plist, char *pdata, size_t isize)
{
    if (plist->icount < 0)
        return false;
    else if (plist->icount == 1)
        return slist_remove_first(plist, pdata, isize);
    else
    {
        ListNode *pnext = plist->pfirst;
        ListNode *pbflast = pnext;
        while (pnext != plist->plast)
        {
            pbflast = pnext;
            pnext = pnext->pnext;
        }
        ListNode *plast = plist->plast;
        plist->plast = pbflast;
        plist->plast->pnext = NULL;
        int iminsize = isize > plast->isize ? plast->isize : isize;
        memcpy(pdata, plast->pdata, iminsize);
        slistnode_free(plast);
        plist->icount--;
        return true;
    }
}

bool slist_remove(SList *plist, unsigned int ipos, char *pdata, size_t isize)
{
    if (ipos >= plist->icount)
        return false;
    else if (ipos == 0)
        return slist_remove_first(plist, pdata, isize);
    else if (ipos == plist->icount - 1)
        return slist_remove_last(plist, pdata, isize);
    else
    {
        ListNode *pbefore = plist->pfirst;
        for (int i = 1; i < ipos; i++)
            pbefore = pbefore->pnext;
        ListNode *pcur = pbefore->pnext;
        ListNode *pafter = pcur->pnext;
        if (pdata != NULL && isize > 0)
        {
            int iminsize = isize > pcur->isize ? pcur->isize : isize;
            memcpy(pdata, pcur->pdata, iminsize);
        }
        slistnode_free(pcur);
        plist->icount--;
        pbefore->pnext = pafter;
    }
    return true;
}

/*
void slist_remove2(SList *plist, unsigned int ipos)
{
    slist_remove(plist, ipos, NULL, 0);
}
*/
