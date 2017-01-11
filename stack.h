#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

typedef struct _StackNode
{
	char *pdata;
    size_t isize;
	struct _StackNode *pnext;
} StackNode;

typedef struct _stack
{
	StackNode *ptop;
	int icount;
} Stack;

Stack* stack_create();
void stack_init(Stack *pstack);

int stack_push(Stack *pstack, char *pdata, size_t isize);
bool stack_pop(Stack *pstack, char *pdata, size_t isize);
void stack_pop2(Stack *pstack);

char* stack_top(Stack *pstack);

bool stack_isempty(Stack *pstack);

void stack_free(Stack *pstack);
void stack_destroy(Stack *pstack);

//void StrStack_print(StrStack *pStack);
#endif
