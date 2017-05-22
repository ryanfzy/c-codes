#include "dict_prvt.h"

KeyedNode* dict_get_item(Dict *pDict, int iNode, int iPos)
{
    KeyedNode *pNode = NULL;
    if (iNode >= 0 && iNode < 256)
    {
        SList *pList = &(pDict->nodes[iNode]);
        int iCount = slist_get_count(pList);
        if (iPos >= 0 && iPos < iCount)
            pNode = (KeyedNode*)slist_get(pList, iPos);
    }
    return pNode;
}
