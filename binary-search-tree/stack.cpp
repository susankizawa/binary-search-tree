#include "stack.h"

#include <stdio.h>
#include <stdlib.h>
#include "bst_non_recur.h"

int stackIsFull(Stack* s){
    if(s->top == (MAX_SIZE - 1))
        return 1;
    else
        return 0;
}

int stackIsEmpty(Stack* s){
    if(s->top == -1)
        return 1;
    else
        return 0;
}

Stack* createStack(){
    Stack* newStack = (Stack*)malloc(sizeof(Stack));
    newStack->top = -1;
    return newStack;
}

void freeStack(Stack* s){
    if(!stackIsEmpty(s)){
        printf("Error: cannot free stack - stack is not empty!\n");
        return;
    }

    free(s);
    printf("Stack successfully freed!\n");

    return;
}

void printStack(Stack* s){
    if(stackIsEmpty(s)){
        printf("Stack is empty.\n");
        return;
    }

    printf("Stack:\n");
    for(int i = s->top;i > -1;i--){
        printNode(s->items[i]);
        printf("\n");
    }
    return;
}

void push(Stack* s, Node* item){
    if(stackIsFull(s)){
        printf("Error: stack is full!\n");
        return;
    }

    // increments top by one then adds an item to the top position;
    s->items[++s->top] = item;
    return;
}

Node* pop(Stack* s){
    if(stackIsEmpty(s)){
        printf("Error: stack is empty!\n");
        return NULL;
    }

    // returns the item on the top position, then decrements top by one;
    return s->items[s->top--];
}
