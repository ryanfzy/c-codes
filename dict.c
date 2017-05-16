#include <string.h>
#include "dict.h"
#include "structs.h"

#define HASH_SIZE 256

static bool do_dict_contains(Dict *pDict, char *pKey, size_t iKeySize, KeyedNode **pNode);

unsigned int hash(char *pKey, size_t iKeySize)
{
    unsigned long ltb = 0;
    for (int i = 0; i < iKeySize; i++)
        ltb += pKey[i];
    /*
    int len = strlen(pkey);
    for (int i = 0; i < len; i++, pkey++)
    {
        int ioffset = *pkey % 26;
        if (*pkey >= 'a' && *pkey <= 'z')
            ioffset = *pkey - 'a';
        else if (*pkey >= 'A' && *pkey <= 'Z')
            ioffset = *pkey - 'A';
        else if (*pkey >= '0' && *pkey <= '9')
            ioffset = (*pkey - '0') * 2;
        ltb |= (1 << ioffset);
    }
    */
    return ltb % HASH_SIZE;
}

Dict* dict_create()
{
    Dict *pdict = malloc(sizeof(Dict));
    dict_init(pdict);
    return pdict;
}

void dict_init(Dict *pdict)
{
    for (int i = 0; i < HASH_SIZE; i++)
        slist_init(&(pdict->nodes[i]));
}

void dict_free(Dict *pdict)
{
    dict_destroy(pdict);
    free(pdict);
}

void dict_destroy(Dict *pdict)
{
    for (int i = 0; i < HASH_SIZE; i++)
        slist_destroy(&(pdict->nodes[i]));
}

void dict_add(Dict *pDict, char *pKey, size_t iKeySize, char *pData, size_t iDataSize)
{
    if (pDict != NULL && pKey != NULL && iKeySize > 0 && 
        !dict_contains(pDict, pKey, iKeySize) && pData != NULL && iDataSize > 0)
    {
        KeyedNode node;
        keyedNode_init(&node, pKey, iKeySize, pData, iDataSize);
        unsigned int hashKey = hash(pKey, iKeySize);
        slist_push(&(pDict->nodes[hashKey]), (char*)&node, sizeof(KeyedNode));
    }
}

void dict_set(Dict *pDict, char *pKey, size_t iKeySize, char *pData, size_t iDataSize)
{
    if (pDict != NULL && pKey != NULL && iKeySize > 0 && pData != NULL && iDataSize > 0)
    {
        KeyedNode *pNode = NULL;
        if (do_dict_contains(pDict, pKey, iKeySize, &pNode))
        {
            keyedNode_destroy(pNode);
            keyedNode_init(pNode, pKey, iKeySize, pData, iDataSize);
        }
    }
}

char* dict_get(Dict *pDict, char *pKey, size_t iKeySize)
{
    if (pDict != NULL && pKey != NULL && iKeySize > 0)
    {
        KeyedNode *pNode = NULL;
        if (do_dict_contains(pDict, pKey, iKeySize, &pNode))
            return pNode->pData;
    }
    return NULL;
}

bool dict_contains(Dict *pDict, char *pKey, size_t iKeySize)
{
    return do_dict_contains(pDict, pKey, iKeySize, NULL);
}

static bool do_dict_contains(Dict *pDict, char *pKey, size_t iKeySize, KeyedNode **ppNode)
{
    if (pDict != NULL && pKey != NULL && iKeySize > 0)
    {
        unsigned int hashKey = hash(pKey, iKeySize);
        SList *pList = &pDict->nodes[hashKey];
        int icount = slist_get_count(pList);
        if (icount > 0)
        {
            for (int i = 0; i < icount; i++)
            {
                KeyedNode *pNode = (KeyedNode*)slist_get(pList, i);
                if (pNode != NULL &&
                    pNode->iKeySize == iKeySize &&
                    memcmp(pNode->pKey, pKey, iKeySize) == 0)
                {
                    if (ppNode != NULL)
                        *ppNode = pNode;
                    return true;
                }
            }
        }
    }
    return false;
}

unsigned int dict_get_count(Dict *pdict)
{
    unsigned int icount = 0;
    if (pdict != NULL)
    {
        for (int i = 0; i < 256; i++)
            icount += slist_get_count(&(pdict->nodes[i]));
    }
    return icount;
}
