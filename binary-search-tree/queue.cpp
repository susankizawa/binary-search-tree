#include "queue.h"

#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

int queueIsEmpty(Queue* q){
    return (q->size == 0);
}

int queueIsFull(Queue* q){
    return (q->size == MAX_SIZE);
}

Queue* createQueue(){
    Queue* newQueue = (Queue*)malloc(sizeof(Queue));
    newQueue->start = 0;
    newQueue->end = -1;
    newQueue->size = 0;
    return newQueue;
}

void freeQueue(Queue* q){
    if(!queueIsEmpty(q)){
        printf("Error: cannot free queue - queue is not empty!\n");
        return;
    }

    free(q);
    printf("Queue successfully freed!\n");

    return;
}

void printQueue(Queue* q){
    if(queueIsEmpty(q)){
        printf("Queue is empty.\n");
        return;
    }

    printf("Queue:\n");

    int p = q->start;
    for(int i = 0; i < q->size; i++){
        printNode(q->items[i]);
        p = (p + 1) % MAX_SIZE;
    }
    printf("\n");
    return;
}

void enqueue(Queue* q, Node* node){
    if(queueIsFull(q)){
        printf("Error: cannot enqueue - queue is full!\n");
        return;
    }

    q->end = (q->end + 1) % MAX_SIZE;
    q->items[q->end] = node;
    q->size++;

    return;
}

Node* dequeue(Queue* q){
    if(queueIsEmpty(q)){
        printf("Error: cannot dequeue - queue is empty!\n");
        return NULL;
    }

    Node* node = q->items[q->start];
    q->start = (q->start + 1) % MAX_SIZE;
    q->size--;

    return node;
}


