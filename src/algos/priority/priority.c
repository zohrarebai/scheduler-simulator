#include "../queue/queue.h"
#include "priority.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "../historicsToDraw/historicsToDraw.h"


void enqueueV3(listPV2 **l, process p) {

   
     
    listPV2 *newNode = (listPV2 *)malloc(sizeof(listPV2));
    
    if (newNode == NULL) {
      
        return;
    }
    newNode->val = p;
    newNode->next = NULL;

  
    if (*l == NULL || (*l)->val.priority < p.priority) {
        newNode->next = *l;
        *l = newNode;
        return;
    }

    
    listPV2 *current = *l;
    while (current->next != NULL && current->next->val.priority >= p.priority) {
        current = current->next;
    }

   
    newNode->next = current->next;
    current->next = newNode;
    
}



process dequeueV3(listPV2 **l) {
    process x = {0};
  
    if (*l != NULL) {
       
        listPV2 *current = *l;
        x = current->val;
         printf("%s leave the ready queue\n",x.name);
        *l = (*l)->next; 
        free(current);
    }
    return x;
}

bool existV2(listPV2 **l, process p) {
    listPV2 *current = *l;
    while (current != NULL) {
        if (strcmp(current->val.name, p.name) == 0) {
            return true;
        }
        current = current->next;
    }
    return false;
}




int compareStructsV3(const void *a, const void *b) {
 
    return ((process*)a)->ta - ((process*)b)->ta;
}


void addNewCommingToQueuev3(listPV2 **l, process *processes , int time , int arraySize,process running,listHistorics ** h){

  for(int i =0 ; i <arraySize ; i++){
      if(processes[i].ta == time && processes[i].te>0 && !existV2(l,processes[i]) && (strcmp(running.name, processes[i].name))){
        printf("%s enter the ready queue at %d\n",processes[i].name,time);
        enqueueV3(l,processes[i]);

          historicsToDraw processEnterReadyQueue ;
                  strcpy(processEnterReadyQueue.nameP, processes[i].name); 
                 processEnterReadyQueue.readyQueueOrRunning = 0;
                  processEnterReadyQueue.enterOrExit = 0;
                  processEnterReadyQueue.time = time;
                 addToHistorics(h,processEnterReadyQueue);
      }
  }
}




listHistorics* priorityX (process * dummy,int nb){

  listHistorics* head=NULL;

  
    size_t arraySize = nb;
    qsort(dummy, arraySize, sizeof(process), compareStructsV3);

  
    listPV2 *readyQueue ;
    
    readyQueue=NULL;

     process current = {"", -1, -1, -1};
    
    int time =0 ;
    int nbProcessFinished =0;
   
   while(nbProcessFinished <nb ){
    
   
  addNewCommingToQueuev3(&readyQueue,dummy,time,arraySize,current,&head);
  

   if(readyQueue!=NULL){
  
     current = dequeueV3(&readyQueue);
    
     int i=0; 
     int tec= current.te ;

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
   
     while(i<tec){
       current.te=current.te-1;
        printf("time %d s: process %s (%d) running \n",i+time,current.name,current.te);

            i++ ;
        
  
      addNewCommingToQueuev3(&readyQueue,dummy,time+i,arraySize,current,&head);
       
         
          if(readyQueue!=NULL){
            if(current.priority < readyQueue->val.priority){
              
                 printf("time %d s: process %s back to ready queueu  \n",i+time,current.name);
                 
                  enqueueV3(&readyQueue,current);

                    historicsToDraw processExitExcution ;
                  strcpy(processExitExcution.nameP, current.name); 
                  processExitExcution.readyQueueOrRunning = 1;
                  processExitExcution.enterOrExit = 1;
                  processExitExcution.time = time+i;
                  addToHistorics(&head,processExitExcution);

                  historicsToDraw processEnterReadyQueue ;
                  strcpy(processEnterReadyQueue.nameP, current.name); 
                  processEnterReadyQueue.readyQueueOrRunning = 0;
                  processEnterReadyQueue.enterOrExit = 0;
                  processEnterReadyQueue.time = time+i;
                  addToHistorics(&head,processEnterReadyQueue);
               
                current=dequeueV3(&readyQueue);
               
                 time+= i;
             
                 i=0;
                tec=current.te;
                  printf("time %d s: process %s take the place  \n",time,current.name);
              
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
            }

          }
  
        }
     
         nbProcessFinished++;
              time+= i;
              printf("%s end !\n",current.name);

               historicsToDraw processFinish;
               strcpy(processFinish.nameP,current.name);
               processFinish.readyQueueOrRunning=1;
               processFinish.enterOrExit=2;
               processFinish.time=time;
               addToHistorics(&head,processFinish);
     }
  
   else{
  

    printf("\ntime %d s:waiting for process .........\n",time);
     time ++ ;
   }

   }
      return head;
}



   
