#ifndef ALGO_LOADER_H
#define ALGO_LOADER_H

#define MAX_ALGOS 20
#define MAX_NAME_LENGTH 50

typedef struct {
    char name[MAX_NAME_LENGTH];
    char display_name[MAX_NAME_LENGTH];
} Algorithm;

typedef struct {
    Algorithm algos[MAX_ALGOS];
    int count;
} AlgorithmList;

int loadAvailableAlgorithms(const char *algo_dir, AlgorithmList *list);


void displayAlgorithmMenu(const AlgorithmList *list);


int getAlgorithmChoice(const AlgorithmList *list);

#endif