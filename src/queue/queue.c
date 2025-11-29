#include "queue.h"
#include <stdlib.h>
#include <string.h>
#include<stdio.h>


void init_queue(queue *q){
    q->tail = NULL ;
    q->head = NULL ;
}

bool enqueue(queue *q,process val){
    node *newNode = (node *)malloc(sizeof(node));
     if(newNode==NULL) return false ;
    newNode->val = val ;
    newNode->next = NULL ;
    if(q->head == NULL){
        q->head = newNode ;

    }else{
        q->tail->next = newNode ;

    }

     q->tail = newNode ;

     return true ;

}

bool stack(queue *q,process val){
    node *newNode = (node *)malloc(sizeof(node));
     if(newNode==NULL) return false ;
    newNode->val = val ;
    newNode->next = q->head ;
    q->head=newNode ;
    q->tail = newNode ;

     return true ;

}



void printQueueContents(queue *q) {
    node *current = q->head;

    while (current != NULL) {
        printf("%s ", current->val.name);
        current = current->next;
    }
}

void printAllQueueContents(queue multilevelQueue[], int nbOfQueue) {
    for (int i = 0; i < nbOfQueue; ++i) {
        printf("Queue %d waiting processes: ", i);
        printQueueContents(&multilevelQueue[i]);
        printf("\n");
    }
}


bool notEmpty(queue *q){
    return !(q->head == NULL) ;
}

process dequeue(queue *q){
    if(q->head == NULL){
        process x = {0};
        return x;
    }

    node *current = q->head;
    process x = current->val;

    q->head = q->head->next;

    free(current);



    if(q->head == NULL) {
        q->tail = NULL;
    }

    return x;
}


bool isProcessInQueue(queue *q, char *name) {
    node *current = q->head;

    while (current != NULL) {
        if (strcmp(current->val.name, name) == 0) {

            return true;
        }
        current = current->next;
    }


    return false;
}