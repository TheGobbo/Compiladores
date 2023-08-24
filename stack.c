#include "stack.h"

#include <stdio.h>
#include <stdlib.h>

Stack* createStack(void** data) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->data = data;
    stack->top = -1;
    return stack;
}

int isEmpty(Stack* stack) { return stack->top == -1; }

int isFull(Stack* stack) { return stack->top == MAX_STACK_SIZE - 1; }

void push(Stack* stack, void* item) {
    if (isFull(stack)) {
        printf("Stack is full. Cannot push.\n");
        return;
    }
    stack->data[++stack->top] = item;
}

void* pop(Stack* stack) {
    if (isEmpty(stack)) {
        printf("Stack is empty. Cannot pop.\n");
        return NULL;
    }
    return stack->data[stack->top--];
}

void* peek(Stack* stack) {
    if (isEmpty(stack)) {
        printf("Stack is empty. Cannot peek.\n");
        return NULL;
    }
    return stack->data[stack->top];
}

void destroyStack(Stack* stack) {
    if (stack != NULL) {
        free(stack);
    }
    return;
}