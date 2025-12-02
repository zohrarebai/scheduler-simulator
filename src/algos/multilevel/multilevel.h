#include "../../Diagramme_Gantt/Diagramme.h"
#include "../../queue/queue.h"
#include <stdbool.h>


int compareStructsV5(const void *a, const void *b);
void swap(int *a, int *b) ;


int nbPriority (process * pTab,int size);

int getRank(process * processes , int priority,int size);



void addNewComingToQueue(queue q[],process *processes,int time,int arraySize,listHistorics ** h);
bool multilevelQueueEmpty(queue q[], int arraySize);

int selectedQueueNumber(queue q[],int arraySize);


bool switchToOtherQueue(queue q[],int arraySize,int currentQueue,int nbOfQueue);

bool shouldWeUseQuantum(queue *q);


listHistorics * multilevelX (process * processes,int arraySize,int quantum);