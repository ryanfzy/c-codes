#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

////////////////////////////////////////////////////
// stack node operations
////////////////////////////////////////////////////
static StackNode* StackNode_new()
{
    StackNode *pNode = malloc(sizeof(StackNode));
    pNode->pCargo = NULL;
    pNode->pNext = NULL;
    pNode->iSize = 0;
    return pNode;
}

static void StackNode_delete(StackNode *pNode)
{
    free(pNode->pCargo);
    //free(pNode->pNext);  // don't free pNext
    free(pNode);
}

////////////////////////////////////////////////////
// stack operations
////////////////////////////////////////////////////

Stack* Stack_new(){
	Stack *pStack = malloc(sizeof(Stack));
    Stack_init(pStack);
	//pStack->top = NULL;
	//pStack->size = 0;
	return pStack;
}

void Stack_init(Stack *pStack)
{
    pStack->pTop = NULL;
    pStack->iSize = 0;
}

void Stack_delete(Stack *pStack)
{
    StackNode *pTopNode = pStack->pTop;
    for (int i = 0; i < pStack->iSize && pTopNode != NULL; i++)
    {
        StackNode *pNextNode = pTopNode->pNext;
        StackNode_delete(pTopNode);
        pTopNode = pNextNode;
    }
    free(pStack);
    pStack = NULL;
}

int Stack_push(Stack* pStack, char *pData, size_t iSize){
	StackNode *pNewNode = StackNode_new();

    pNewNode->iSize = iSize;
	pNewNode->pCargo = malloc(pNewNode->iSize);
    memcpy(pNewNode->pCargo, pData, pNewNode->iSize);
    //pNewNode->cargo[pNewNode->iSize] = '\0';

	if(pStack->iSize == 0)
        pNewNode->pNext = NULL;
	else
		pNewNode->pNext = pStack->pTop;

    pStack->pTop = pNewNode;
	pStack->iSize++;

	return pStack->iSize;
}

bool Stack_pop(Stack *pStack, char *pData, size_t iSize){
	if(pStack->iSize == 0)
		return false; 

	StackNode *pNode = pStack->pTop;
    int iMinSize = iSize > pNode->iSize ? pNode->iSize : iSize;
    //memcpy(szStr, pNode->cargo, iMinSize - 1);
    memcpy(pData, pNode->pCargo, iMinSize);
    //szStr[iMinSize] = '\0';

	pStack->pTop = pNode->pNext;
	pStack->iSize--;

    StackNode_delete(pNode); 
    return true;
}

bool Stack_isEmpty(Stack *pStack){
	return pStack->iSize == 0;
}

/*
void Stack_print(Stack *stack){
	StackNode *node;
	node = stack->top;
	printf("[%d]", stack->size);
	while(node){
		printf("(%d)", *((int*)(node->cargo)));
		node = node->next;
	}
	printf("\n");
}*/

