#include <string.h>
#include "dict.h"
#include "structs.h"

#define HASH_SIZE 256

static bool do_dict_contains(Dict *pDict, char *szKey, NamedNode **pNode);

unsigned int hash(char *pkey)
{
    unsigned long ltb = 0;
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

void dict_add(Dict *pdict, char *pkey, char *pvalue, size_t ivalsize)
{
    if (pdict != NULL && strlen(pkey) > 0 && !dict_contains(pdict, pkey))
    {
        NamedNode node;
        namedNode_init(&node, pkey, pvalue, ivalsize);
        unsigned int hkey = hash(pkey);
        slist_push(&(pdict->nodes[hkey]), (char*)&node, sizeof(NamedNode));
    }
}

void dict_set(Dict *pDict, char *szKey, char *pValue, size_t iValSize)
{
    NamedNode *pNode = NULL;
    if (pDict != NULL && strlen(szKey) > 0 && do_dict_contains(pDict, szKey, &pNode))
    {
        namedNode_destroy(pNode);
        namedNode_init(pNode, szKey, pValue, iValSize);
    }
}

char* dict_get(Dict *pdict, char *pkey)
{
    NamedNode *pnode = NULL;
    if (pdict != NULL && do_dict_contains(pdict, pkey, &pnode))
        return pnode->pData;
    return NULL;
}

bool dict_contains(Dict *pdict, char *pkey)
{
    return do_dict_contains(pdict, pkey, NULL);
}

static bool do_dict_contains(Dict *pdict, char *pkey, NamedNode **ppnode)
{
    if (pdict != NULL && strlen(pkey) > 0)
    {
        unsigned int hkey = hash(pkey);
        SList *plist = &pdict->nodes[hkey];
        int icount = slist_get_count(plist);
        if (icount > 0)
        {
            for (int i = 0; i < icount; i++)
            {
                NamedNode *pnode = (NamedNode*)slist_get(plist, i);
                if (pnode != NULL && strcmp(pnode->szName, pkey) == 0)
                {
                    if (ppnode != NULL)
                        *ppnode = pnode;
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
