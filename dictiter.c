#include "dictiter.h"
#include "dict_prvt.h"

static int max(int a, int b)
{
    return (a < b) ? b : a;
}

static DictIter* dictiter_create()
{
    DictIter *pIter = malloc(sizeof(DictIter));
    pIter->pDict = NULL;
    pIter->iNode = -1;
    pIter->iPos = -1;
    return pIter;
}

void dictiter_free(DictIter *pIter)
{
    free(pIter);
}

DictIter* dict_get_iter(Dict *pDict)
{
    DictIter *pIter = dictiter_create();
    pIter->pDict = pDict;
    return pIter;
}

char* dictiter_get_key(DictIter *pIter)
{
    KeyedNode *pNode = dict_get_item(pIter->pDict, pIter->iNode, pIter->iPos);
    if (pNode != NULL)
        return pNode->pKey;
    return NULL;
}

char* dictiter_get_value(DictIter *pIter)
{
    KeyedNode *pNode = dict_get_item(pIter->pDict, pIter->iNode, pIter->iPos);
    if (pNode != NULL)
        return pNode->pData;
    return NULL;
}

bool dictiter_move_next(DictIter *pIter)
{
    bool bHasNext = false;
    if (pIter != NULL)
    {
        int iNode = max(pIter->iNode, 0);
        int iPos = pIter->iPos + 1;
        if (iNode < 256)
        {
            SList *pList = &(pIter->pDict->nodes[iNode]);
            int iCount = slist_get_count(pList);

            if (iPos >= iCount-1)
            {
                for (iNode = iNode+1, iPos = 0; iNode < 256; iNode++)
                {
                    pList = &(pIter->pDict->nodes[iNode]);
                    if (slist_get_count(pList) > 0)
                        break;
                }
            }

            if (iNode < 256)
            {
                pIter->iNode = iNode;
                pIter->iPos = iPos;
                bHasNext = true;
            }
        }
    }
    return bHasNext;
}
