#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include "list.h"
#define MAX 2048


//First integer stack
typedef struct stack{
	int i;
	list *l[MAX];

}stack;

void push(stack *s, list *l);
list *pop(stack *s);
int empty(stack *s);
int full(stack *s);
void sinit(stack *s);


// Second interger stack
typedef struct stack1{
	int i;
	list *l[MAX];

}stack1;

void push1(stack1 *s, list *l);
list *pop1(stack1 *s);
int empty1(stack1 *s);
int full1(stack1 *s);
void sinit1(stack1 *s);


// Character stack
typedef struct cstack{
	char a[MAX];
	int i;
}cstack;

void cpush(cstack *s, char num);
char cpop(cstack *s);
int cempty(cstack *s);
int cfull(cstack *s);
void cinit(cstack *s);


#endif // STACK_H_INCLUDED
