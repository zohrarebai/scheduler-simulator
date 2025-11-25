#include "Diagramme.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void addToHistorics(listHistorics **l, historicsToDraw h) {
 
    listHistorics *newNode = (listHistorics *)malloc(sizeof(listHistorics));
   
    if (newNode == NULL) {
        return;
    }
    newNode->val = h;
    newNode->next = NULL;

    if (*l == NULL) {
        *l = newNode; 
    } else {
        listHistorics *tail = *l;
        while (tail->next != NULL) {
            tail = tail->next;
        }
        tail->next = newNode; 
    }
}

int getIndexOfProcess(process *processes,int nbOfProcesses,char *name){
    for(int i =0 ; i<nbOfProcesses; i++){
        if(strcmp(name,processes[i].name)==0)
                return i ;
    }
    return 0 ;

}

void concatenateProcessNames(char *destination, const char *source) {
    if(strcmp(destination,""))
    strcat(destination, ","); 
    strcat(destination, source);
  
}





void printQueue(listQueueState *head) {
    printf("Queue State\n");
    while (head != NULL) {
        printf("at time :%d in queue there are processes: %s\n", head->val.nb,head->val.namesOfProcessess);
        head = head->next;
    }
}

void removeDuplicates(listQueueState **headRef) {
    listQueueState *current = *headRef;
    listQueueState *newList = NULL;
    listQueueState *tail = NULL;

    while (current != NULL) {
        listQueueState *temp = current->next;
        listQueueState *lastOccurrence = current;

        while (temp != NULL) {
            if (temp->val.time == current->val.time) {
                lastOccurrence = temp;
            }
            temp = temp->next;
        }

        if (lastOccurrence != NULL) {
            if (newList == NULL) {
                newList = malloc(sizeof(listQueueState));
                if (newList == NULL) {
              
                    return;
                }
                strcpy(newList->val.namesOfProcessess, lastOccurrence->val.namesOfProcessess);
                newList->val.time = lastOccurrence->val.time;
                newList->next = NULL;
                tail = newList;
            } else {
                tail->next = malloc(sizeof(listQueueState));
                if (tail->next == NULL) {
                  
                    return;
                }
                tail = tail->next;
                strcpy(tail->val.namesOfProcessess, lastOccurrence->val.namesOfProcessess);
                tail->val.time = lastOccurrence->val.time;
                tail->val.nb = lastOccurrence->val.nb;
                tail->next = NULL;
            }
            current = lastOccurrence->next;
        } else {
            current = current->next;
        }
    }

    while (*headRef != NULL) {
        listQueueState *temp = *headRef;
        *headRef = (*headRef)->next;
        free(temp);
    }

    *headRef = newList;
}






listQueueState * setWhoIsInTheQueue(listHistorics *l,int nbOfProcesses,process * processes) {
    int existInqueueState [nbOfProcesses];
    for(int i = 0 ; i<nbOfProcesses ; i++){
        existInqueueState[i]=0;
    }
    listQueueState * list =NULL ;

     listHistorics *current = l;

    while (current != NULL) {
        if((current->val.readyQueueOrRunning==0 && current->val.enterOrExit==1) ||(current->val.readyQueueOrRunning==0 && current->val.enterOrExit==0) ){

    listQueueState *newNode = (listQueueState *)malloc(sizeof(listQueueState));
    newNode->next = NULL ;
    strcpy(newNode->val.namesOfProcessess,"");

   if(current->val.enterOrExit==0){
        existInqueueState[getIndexOfProcess(processes,nbOfProcesses,current->val.nameP)]=1 ;
   }else{
         existInqueueState[getIndexOfProcess(processes,nbOfProcesses,current->val.nameP)]=0 ;
   }
    int nb =0 ;
    
    for(int i =0;i<nbOfProcesses;i++){
        if(existInqueueState[i]==1){
            concatenateProcessNames(newNode->val.namesOfProcessess, processes[i].name);
            nb++;
         
        }
    }
    
  
    newNode->val.time = current->val.time ;
    newNode->val.nb=nb;
  

      if (list == NULL) {
        list = newNode; 
        } else {
        listQueueState *tail = list;
        while (tail->next != NULL) {
            tail = tail->next;
        }
        tail->next = newNode; 
        }
    }
     

        current = current->next;
    }
 
    removeDuplicates(&list);
    printQueue(list);
    return list ;
}

void printList(listHistorics *head) {
    listHistorics *current = head;

    while (current != NULL) {
        const char* sentence = NULL;

    
        if (current->val.readyQueueOrRunning == 0 && current->val.enterOrExit== 0) {
            sentence = "Entered the ready queue.";
        } else if (current->val.readyQueueOrRunning == 0 && current->val.enterOrExit == 1) {
            sentence = "Exited the ready queue.";
        } else if (current->val.readyQueueOrRunning == 1 && current->val.enterOrExit == 0) {
            sentence = "Entered execution.";
        } else if (current->val.readyQueueOrRunning == 1 && current->val.enterOrExit == 1) {
            sentence = "Exited execution.";
        } else if (current->val.readyQueueOrRunning == 1 && current->val.enterOrExit== 2) {
            sentence = "Process ended.";
        } else {
        
            sentence = "Unknown action.";
        }

        printf("Name: %s, Action: %s, Time: %d\n",
               current->val.nameP,
               sentence,
               current->val.time);

        current = current->next;
    }
}