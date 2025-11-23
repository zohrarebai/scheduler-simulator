#include "../queue/queue.h"
#include "../historicsToDraw/historicsToDraw.h"


typedef struct listPV2
{
  process val ;
  struct listPV2 *next ;
} listPV2 ;






void enqueueV3(listPV2 **l, process p) ;
process dequeueV3(listPV2 **l);

listHistorics*  priorityX (process * dummy,int nb);
int compareStructs(const void *a, const void *b) ;
void addNewCommingToQueuev3(listPV2 **l, process *processes , int time , int arraySize,process running,listHistorics ** h);