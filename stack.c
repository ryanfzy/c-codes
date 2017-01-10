#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

////////////////////////////////////////////////////
// stack node operations
////////////////////////////////////////////////////
static StackNode* stacknode_create()
{
    StackNode *pnode = malloc(sizeof(StackNode));
    pnode->pdata = NULL;
    pnode->pnext = NULL;
    pnode->isize = 0;
    return pnode;
}

static void stacknode_free(StackNode *pnode)
{
    free(pnode->pdata);
    //free(pNode->pNext);  // don't free pNext
    free(pnode);
}

////////////////////////////////////////////////////
// stack operations
////////////////////////////////////////////////////

Stack* stack_create(){
	Stack *pstack = malloc(sizeof(Stack));
    stack_init(pstack);
	return pstack;
}

void stack_init(Stack *pstack)
{
    pstack->ptop = NULL;
    pstack->icount = 0;
}

void stack_free(Stack *pstack)
{
    stack_destroy(pstack);
    free(pstack);
    //pstack = NULL;
}

void stack_destroy(Stack *pstack)
{
    StackNode *ptop = pstack->ptop;
    for (int i = 0; i < pstack->icount && ptop != NULL; i++)
    {
        StackNode *pnext = ptop->pnext;
        stacknode_free(ptop);
        ptop = pnext;
    }

    pstack->ptop = NULL;
    pstack->icount = 0;
}

int stack_push(Stack* pstack, char *pdata, size_t isize){
	StackNode *pnew = stacknode_create();

    pnew->isize = isize;
	pnew->pdata = malloc(pnew->isize);
    memcpy(pnew->pdata, pdata, pnew->isize);

	if(pstack->icount == 0)
        pnew->pnext = NULL;
	else
		pnew->pnext = pstack->ptop;

    pstack->ptop = pnew;
	pstack->icount++;

	return pstack->icount;
}

bool stack_pop(Stack *pstack, char *pdata, size_t isize){
	if(pstack->icount == 0)
		return false; 

	StackNode *pnode = pstack->ptop;
    int iminsize = isize > pnode->isize ? pnode->isize : isize;
    memcpy(pdata, pnode->pdata, iminsize);

	pstack->ptop = pnode->pnext;
	pstack->icount--;

    stacknode_free(pnode); 
    return true;
}

void stack_pop2(Stack *pstack)
{
    if (pstack->ptop != NULL)
    {
        StackNode *pnode = pstack->ptop;
        pstack->ptop = pnode->pnext;
        pstack->icount--;
        stacknode_free(pnode);
    }
}

bool stack_isempty(Stack *pstack){
	return pstack->icount == 0;
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

