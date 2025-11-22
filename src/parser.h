#ifndef PARSER_H
#define PARSER_H

typedef struct {
    char name[50];
    int ta;
    int te;
    int priority;
} process;

int parseConfigFile(const char *filename, process **pTab, int *arraySize);


int generateConfigFile(const char *filename, int nbProcesses,
                       int minTa, int maxTa,
                       int minTe, int maxTe,
                       int minP, int maxP);


void freeProcessArray(process *pTab);

#endif