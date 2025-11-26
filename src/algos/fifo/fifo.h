#include "../queue/queue.h"
#include "../../Diagramme_Gantt/Diagramme.h"

int compareStructs(const void *a, const void *b);
void addNewCommingToQueue(queue *q, process *process, int time, int arraySize, listHistorics **l);

listHistorics* fifoX(process *dummy, int nb);
