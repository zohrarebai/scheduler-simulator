#include "../../queue/queue.h"
#include "fifo.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<string.h>


int compareStructs(const void *a, const void *b) {
    return ((process*)a)->ta - ((process*)b)->ta;
}




void  addNewCommingToQueue(queue *q,process *process,int time,int arraySize,listHistorics ** l){
    for(int i=0; i<arraySize ; i++){
     
        if(process[i].ta==time){
               printf(" at the time %d s: %s enter the ready queue !\n",time,process[i].name);
                 enqueue(q,process[i]);

                 historicsToDraw processEnterReadyQueue ;
                  strcpy(processEnterReadyQueue.nameP, process[i].name); 
                 processEnterReadyQueue.readyQueueOrRunning = 0;
                  processEnterReadyQueue.enterOrExit = 0;
                  processEnterReadyQueue.time = time;
                 addToHistorics(l,processEnterReadyQueue);
                 
                 process[i].ta = -1 ;

        }
          
    }

}




listHistorics* fifoX (process * dummy,int nb){

  listHistorics* head=NULL;

  
    size_t arraySize = nb;
    qsort(dummy, arraySize, sizeof(process), compareStructs);


    queue q ;
    init_queue(&q);

    process current ;
    int time =0 ;
    
   int nbProcessFinished =0;
   while( nbProcessFinished<nb){
   
  addNewCommingToQueue(&q,dummy,time,arraySize,&head);

   if(notEmpty(&q)){
     current = dequeue(&q);

     historicsToDraw processExitReadyQueue;
      strcpy(processExitReadyQueue.nameP,current.name);
       processExitReadyQueue.readyQueueOrRunning=0;
         processExitReadyQueue.enterOrExit=1;
         processExitReadyQueue.time=time;
     addToHistorics(&head,processExitReadyQueue);


      historicsToDraw processEnterExecution= {*current.name,1,0,time};
      strcpy(processEnterExecution.nameP,current.name);
      processEnterExecution.readyQueueOrRunning = 1 ;
      processEnterExecution.enterOrExit=0 ;
      processEnterExecution.time=time;
     addToHistorics(&head,processEnterExecution);



     for(int i=time;i<time+current.te;i++){
        addNewCommingToQueue(&q,dummy,i,arraySize,&head);
        printf("time %d s: process %s running \n",i,current.name);
    
     }
    
     time+= current.te ;
     nbProcessFinished ++ ;
     historicsToDraw processFinish;
     strcpy(processFinish.nameP,current.name);
     processFinish.readyQueueOrRunning=1;
     processFinish.enterOrExit=2;
     processFinish.time=time;
     addToHistorics(&head,processFinish);
     printf("\n =================================== \nat time %d s: process %s end ! \n \n =================================== \n",time,current.name);
   
   }else{

    printf("\ntime %d s:waiting for process .........\n",time);
     time ++ ;
   }

  
   }

    return head;
}