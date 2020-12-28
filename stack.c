#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "list.h"

/*  1> Programmer should check that stack is not empty before calling pop()
    2> It is mandatory to call init() on a stack before calling push, pop or any other function on the stack
 */

void push(stack *s, list *l){
	s->l[s->i]= l;
	(s->i)++;
}

list *pop(stack *s){
	list *t = (list *)malloc(sizeof(list));
	t = s->l[s->i - 1];
	(s->i)--;
	return t;
}

int empty(stack *s){
	return s->i == 0;
}

int full(stack *s){
	return s->i == MAX;
}

void sinit(stack *s){
	s->i = 0;
}




void push1(stack1 *s, list *l){
	s->l[s->i]= l;
	(s->i)++;
}

list *pop1(stack1 *s){
	list *t = (list *)malloc(sizeof(list));
	t = s->l[s->i - 1];
	(s->i)--;
	return t;
}

int empty1(stack1 *s){
	return s->i == 0;
}

int full1(stack1 *s){
	return s->i == MAX;
}

void sinit1(stack1 *s){
	s->i = 0;
}





void cpush(cstack *s, char num) {
	s->a[s->i]= num;
	(s->i)++;
}

char cpop(cstack *s) {
	char t = s->a[s->i - 1];
	(s->i)--;
	return t;
}

int cempty(cstack *s) {
	return s->i == 0;
}

int cfull(cstack *s) {
	return s->i == MAX;
}

void cinit(cstack *s) {
	s->i = 0;
}
