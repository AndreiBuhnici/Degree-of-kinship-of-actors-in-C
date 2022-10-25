#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node_q{
	int data;
	struct node_q* next;
}node_q;

 
typedef struct Queue{
	struct node_q *tail , *head;
	int len;
}Queue;


void init_queue(Queue **q);
void free_queue(Queue** q);
int add_queue(Queue * q, int val);
int pop_queue(Queue *q);
void print_queue(Queue *q);
int front(Queue *q);
int isEmptyQueue(Queue *q);
#endif