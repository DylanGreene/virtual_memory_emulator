#include "queue.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


struct Node{
    int data;
    struct Node *next;
    struct Node *prev;
};

struct Queue{
    struct Node *head;
    struct Node *tail;
    int size;
};

struct Queue * create_queue(){
    struct Queue *q;
    q = malloc(sizeof(*q));
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

void delete_queue(struct Queue *q){
    while(q->size){
        queue_pop(q);
    }
    free(q);
}

void queue_push(struct Queue *q, int data){
    struct Node *new_node;
    new_node = malloc(sizeof(*new_node));

    new_node->data = data;
    new_node->next = q->head;
    new_node->prev = NULL;

    if(q->size != 0){
        (q->head)->prev = new_node;
    }
    q->head = new_node;
    if(q->size == 0){
        q->tail = q->head;
    }
    q->size = q->size + 1;
}

void queue_pop(struct Queue *q){
    if(q->size == 0){
        return;
    }
    struct Node *new_tail = q->tail->prev;
    free(q->tail);
    q->tail = new_tail;
    q->size = q->size - 1;
}

int queue_front(struct Queue *q){
    return q->tail->data;
}

int queue_size(struct Queue *q){
    return q->size;
}
