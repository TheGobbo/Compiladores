
#if !defined(STACK_H)
#define STACK_H

#include <stdio.h>

#define MAXSTACK_SIZE 8

typedef struct {
    int stack[MAXSTACK_SIZE];
    int top;
} Stack;

Stack* new_Stack();
void delete_Stack(Stack* s);

int stack_isempty(Stack* s);
int stack_isfull(Stack* s);
int stack_pop(Stack* s);
int stack_push(Stack* s, int data);

#endif  // STACK_H
