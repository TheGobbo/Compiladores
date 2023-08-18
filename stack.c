#include "stack.h"

#include <stdlib.h>

Stack* new_Stack() {
    printf("NEW STACK CREATED!\n");
    Stack* s = malloc(sizeof(Stack));
    s->top = -1;
    return s;
}

void delete_Stack(Stack* s) {
    if (s != NULL) {
        free(s);
        printf("STACK DELETED!\n");
    }
    return;
}

/* Check if the stack is empty */
int stack_isempty(Stack* s) {
    if (s->top == -1)
        return 1;
    else
        return 0;
}

/* Check if the stack is full */
int stack_isfull(Stack* s) {
    if (s->top == MAXSTACK_SIZE)
        return 1;
    else
        return 0;
}

/* Function to return the topmost element in the stack */
int stack_peek(Stack* s) {
    if (!stack_isempty(s)) {
        return s->stack[s->top];
    } else {
        printf("Could not retrieve data, Stack is empty.\n");
    }
}

/* Function to delete from the stack */
int stack_pop(Stack* s) {
    int data;
    if (!stack_isempty(s)) {
        data = s->stack[s->top];
        s->top = s->top - 1;
        return data;
    } else {
        printf("Could not retrieve data, Stack is empty.\n");
    }
}

/* Function to insert into the stack */
int stack_push(Stack* s, int data) {
    if (!stack_isfull(s)) {
        s->top = s->top + 1;
        s->stack[s->top] = data;
    } else {
        printf("Could not insert data, Stack is full.\n");
    }
}
