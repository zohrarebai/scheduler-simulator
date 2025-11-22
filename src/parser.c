#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"


int parseConfigFile(const char *filename, process **pTab, int *arraySize) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        fprintf(stderr, "Erreur: Impossible d'ouvrir le fichier %s\n", filename);
        return -1;
    }

    char buffer[100];
    process tab[100];
    int i = 0;

    while (fgets(buffer, 100, f)) {

        if (buffer[0] == '#' || isspace(buffer[0])) {
            continue;
        }

        char *token = strtok(buffer, ",");
        if (token == NULL) continue;
        strcpy(tab[i].name, token);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        tab[i].ta = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        tab[i].te = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        tab[i].priority = atoi(token);

        i++;
    }

    fclose(f);

    *arraySize = i;
    *pTab = (process *)malloc(i * sizeof(process));

    if (*pTab == NULL) {
        fprintf(stderr, "Erreur: Allocation mémoire échouée\n");
        return -1;
    }

    memcpy(*pTab, tab, i * sizeof(process));

    return 0;
}


int generateConfigFile(const char *filename, int nbProcesses,
                       int minTa, int maxTa,
                       int minTe, int maxTe,
                       int minP, int maxP) {

    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        fprintf(stderr, "Erreur: Impossible de créer le fichier %s\n", filename);
        return -1;
    }

    fprintf(f, "# Fichier de configuration généré automatiquement\n");
    fprintf(f, "# Format: nom, arrival_time, execution_time, priority\n\n");

    for (int i = 1; i <= nbProcesses; i++) {
        char name[12];
        snprintf(name, 12, "P%d", i);

        int randomTa = rand() % (maxTa - minTa + 1) + minTa;
        int randomTe = rand() % (maxTe - minTe + 1) + minTe;
        int randomP = rand() % (maxP - minP + 1) + minP;

        fprintf(f, "%s,%d,%d,%d\n", name, randomTa, randomTe, randomP);
    }

    fclose(f);
    return 0;
}


void freeProcessArray(process *pTab) {
    if (pTab != NULL) {
        free(pTab);
    }
}