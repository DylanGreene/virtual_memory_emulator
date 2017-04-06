// Queue based on a linked list

#ifndef QUEUE_H
#define QUEUE_H

struct Queue;

struct Queue * create_queue();
void delete_queue(struct Queue *q);

void queue_push(struct Queue *q, int data);
void queue_pop(struct Queue *q);
int queue_front(struct Queue *q);

int queue_size(struct Queue *q);

#endif
