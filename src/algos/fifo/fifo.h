#include "../queue/queue.h"
#include "../historicsToDraw/historicsToDraw.h"

int compareStructs(const void *a, const void *b) ;
void addNewCommingToQueue(queue *q,process *process,int time,int arraySize,listHistorics ** l);

listHistorics* fifoX(process * dummy,int nb);




