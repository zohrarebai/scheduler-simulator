#include "../../queue/queue.h"
#include "rr.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<string.h>
int compareStructsv8(const void *a, const void *b) {
    return ((process*)a)->ta - ((process*)b)->ta;
}



void  addNewCommingToQueuev8(queue *q,process *process,int time,int arraySize,listHistorics ** l){
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




listHistorics* roundRobinX (process * dummy,int nb,int qu){
  
    printf("DEBUG: Received %d processes:\n", nb);
    for(int i = 0; i < nb; i++) {
        printf("  Process %d: name='%s', ta=%d, te=%d\n", 
               i, dummy[i].name, dummy[i].ta, dummy[i].te);
    }

  listHistorics* head=NULL;


    size_t arraySize = nb;
    qsort(dummy, arraySize, sizeof(process), compareStructsv8);


    queue q ;
    init_queue(&q);

    process current ;
    int time =0 ;

   int nbProcessFinished =0;
   while( nbProcessFinished<nb){

  addNewCommingToQueuev8(&q,dummy,time,arraySize,&head);

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

    int quantum = 0 ;
    int teBeforeRunnig=current.te ;

     while(current.te>0 && quantum<qu){
       current.te = current.te -1 ;
        quantum ++;
        addNewCommingToQueuev8(&q,dummy,time+quantum,arraySize,&head);
        printf("time %d s: process %s running \n", time+quantum, current.name);
     }

     if(current.te==0){

     time+= teBeforeRunnig ;
     historicsToDraw processFinish;
     strcpy(processFinish.nameP,current.name);
     processFinish.readyQueueOrRunning=1;
     processFinish.enterOrExit=2;
     processFinish.time=time;
     addToHistorics(&head,processFinish);
     printf("\n =================================== \nat time %d s: process %s end ! \n \n =================================== \n",time,current.name);
    nbProcessFinished++ ;
     }else{
        time+= quantum ;
        enqueue(&q,current);
        printf("at %d : %s exit execution and back to queue\n", time, current.name);
           historicsToDraw processExitExcution ;
                  strcpy(processExitExcution.nameP, current.name);
                  processExitExcution.readyQueueOrRunning = 1;
                  processExitExcution.enterOrExit = 1;
                  processExitExcution.time = time;
                  addToHistorics(&head,processExitExcution);

                  historicsToDraw processEnterReadyQueue ;
                  strcpy(processEnterReadyQueue.nameP, current.name);
                  processEnterReadyQueue.readyQueueOrRunning = 0;
                  processEnterReadyQueue.enterOrExit = 0;
                  processEnterReadyQueue.time = time;
                  addToHistorics(&head,processEnterReadyQueue);
     }

   }else{

    printf("\ntime %d s:waiting for process .........\n",time);
     time ++ ;
   }


   }

    return head;
}