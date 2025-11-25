#ifndef HISTORICSTODRAW_H
#define HISTORICSTODRAW_H
 #include "../queue/queue.h"

typedef struct historicsToDraw
{
   char nameP [100];
   int readyQueueOrRunning ;
   int enterOrExit ;
   int time;
} historicsToDraw;

typedef struct listHistorics
{
  historicsToDraw val ;
  struct listHistorics *next ;
} listHistorics ;

typedef struct queueState
{
   char namesOfProcessess[100];
   int time;
   int nb;
} queueState;

typedef struct listQueueState
{
 queueState val ;
  struct listQueueState *next ;
} listQueueState ;


typedef struct TwoInOne
{

   listHistorics *list ;
   listQueueState *queueList ;
  
} TwoInOne;


void addToHistorics(listHistorics **l, historicsToDraw h) ;
listQueueState * setWhoIsInTheQueue(listHistorics *l,int nbOfProcesses,process * processess) ;
void printList(listHistorics *head);

#endif

