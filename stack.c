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
	return pStack;
}

void Stack_init(Stack *pStack)
{
    pStack->pTop = NULL;
    pStack->iCount = 0;
}

void Stack_delete(Stack *pStack)
{
    Stack_destroy(pStack);
    /*
    StackNode *pTopNode = pStack->pTop;
    for (int i = 0; i < pStack->iSize && pTopNode != NULL; i++)
    {
        StackNode *pNextNode = pTopNode->pNext;
        StackNode_delete(pTopNode);
        pTopNode = pNextNode;
    }
    */
    free(pStack);
    pStack = NULL;
}

void Stack_destroy(Stack *pStack)
{
    StackNode *pTopNode = pStack->pTop;
    for (int i = 0; i < pStack->iCount && pTopNode != NULL; i++)
    {
        StackNode *pNextNode = pTopNode->pNext;
        StackNode_delete(pTopNode);
        pTopNode = pNextNode;
    }

    pStack->pTop = NULL;
    pStack->iCount = 0;
}

int Stack_push(Stack* pStack, char *pData, size_t iSize){
	StackNode *pNewNode = StackNode_new();

    pNewNode->iSize = iSize;
	pNewNode->pCargo = malloc(pNewNode->iSize);
    memcpy(pNewNode->pCargo, pData, pNewNode->iSize);

	if(pStack->iCount == 0)
        pNewNode->pNext = NULL;
	else
		pNewNode->pNext = pStack->pTop;

    pStack->pTop = pNewNode;
	pStack->iCount++;

	return pStack->iCount;
}

bool Stack_pop(Stack *pStack, char *pData, size_t iSize){
	if(pStack->iCount == 0)
		return false; 

	StackNode *pNode = pStack->pTop;
    int iMinSize = iSize > pNode->iSize ? pNode->iSize : iSize;
    memcpy(pData, pNode->pCargo, iMinSize);

	pStack->pTop = pNode->pNext;
	pStack->iCount--;

    StackNode_delete(pNode); 
    return true;
}

bool Stack_isEmpty(Stack *pStack){
	return pStack->iCount == 0;
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

