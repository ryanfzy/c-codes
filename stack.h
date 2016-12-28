#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

typedef struct _stackNode
{
	char *pCargo;
    size_t iSize;
	struct _stackNode *pNext;
} StackNode;

typedef struct _stack
{
	StackNode *pTop;
	int iSize;
} Stack;

Stack* Stack_new();
void Stack_init(Stack *pStack);
int Stack_push(Stack* pStack, char *pData, size_t iSize);
bool Stack_pop(Stack* pStack, char *pData, size_t iSize);
bool Stack_isEmpty(Stack *pStack);
void Stack_delete(Stack *pStack);

//void StrStack_print(StrStack *pStack);
#endif
