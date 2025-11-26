#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <limits.h>
#include "../../parser.h"

#define EMPTY_QUEUE INT_MIN
#include <stdbool.h>






typedef struct node
{
  process val ;
  struct node *next ;
} node ;

typedef struct queue
{
    node *head;
    node *tail;
} queue;




void init_queue(queue *q);

bool enqueue(queue *q,process val);
bool notEmpty(queue *q);
bool stack(queue *q,process val);

process dequeue(queue *q);
bool isProcessInQueue(queue *q, char *name) ;
void printQueueContents(queue *q) ;
void printAllQueueContents(queue multilevelQueue[], int nbOfQueue) ;
#endif