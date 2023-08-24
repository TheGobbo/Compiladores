#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#define MAX_STACK_SIZE 100  // You can adjust this as needed

typedef struct Stack {
    void** data;
    int top;
} Stack;

Stack* createStack(void** data);
int isEmpty(Stack* stack);
int isFull(Stack* stack);
void push(Stack* stack, void* item);
void* pop(Stack* stack);
void* peek(Stack* stack);
void destroyStack(Stack* stack);

#endif
