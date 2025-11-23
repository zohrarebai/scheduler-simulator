#include "../queue/queue.h"
#include "../historicsToDraw/historicsToDraw.h"
int compareStructsv8(const void *a, const void *b) ;
void addNewCommingToQueuev8(queue *q,process *process,int time,int arraySize,listHistorics ** l);

listHistorics* roundRobinX (process * dummy,int nb,int qu);