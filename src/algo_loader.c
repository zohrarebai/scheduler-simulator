#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>
#include "algo_loader.h"


static void formatDisplayName(const char *folder_name, char *display_name) {
    int i = 0, j = 0;
    int capitalize_next = 1;

    while (folder_name[i] != '\0' && j < MAX_NAME_LENGTH - 1) {
        if (folder_name[i] == '_' || folder_name[i] == '-') {
            display_name[j++] = ' ';
            capitalize_next = 1;
        } else {
            if (capitalize_next) {
                display_name[j++] = toupper(folder_name[i]);
                capitalize_next = 0;
            } else {
                display_name[j++] = folder_name[i];
            }
        }
        i++;
    }
    display_name[j] = '\0';
}


static int isDirectory(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0) {
        return 0;
    }
    return S_ISDIR(statbuf.st_mode);
}


int loadAvailableAlgorithms(const char *algo_dir, AlgorithmList *list) {
    DIR *dir;
    struct dirent *entry;
    char full_path[512];

    list->count = 0;

    dir = opendir(algo_dir);
    if (dir == NULL) {
        fprintf(stderr, "Erreur: Impossible d'ouvrir le dossier %s\n", algo_dir);
        return -1;
    }

    // Parcourir tous les éléments
    while ((entry = readdir(dir)) != NULL && list->count < MAX_ALGOS) {

        // Ignorer . et ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Construire le chemin complet
        int path_len = snprintf(full_path, sizeof(full_path), "%s/%s", algo_dir, entry->d_name);

        // Vérifier si le chemin a été tronqué
        if (path_len >= (int)sizeof(full_path)) {
            fprintf(stderr, "Avertissement: Chemin trop long pour %s, ignoré\n", entry->d_name);
            continue;
        }

        if (isDirectory(full_path)) {
            strncpy(list->algos[list->count].name, entry->d_name, MAX_NAME_LENGTH - 1);
            list->algos[list->count].name[MAX_NAME_LENGTH - 1] = '\0';

            formatDisplayName(entry->d_name, list->algos[list->count].display_name);

            list->count++;
        }
    }

    closedir(dir);
    return list->count;
}


void displayAlgorithmMenu(const AlgorithmList *list) {
    printf("\n╔═══════════════════════════════════════════════════╗\n");
    printf("║      ALGORITHMES D'ORDONNANCEMENT DISPONIBLES    ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n\n");

    if (list->count == 0) {
        printf("❌ Aucun algorithme trouvé dans le dossier algos/\n");
        return;
    }

    for (int i = 0; i < list->count; i++) {
        printf("  %2d. %-30s 📊\n", i + 1, list->algos[i].display_name);
    }

    printf("\n   0. Quitter\n");
    printf("\n═══════════════════════════════════════════════════\n");
}


int getAlgorithmChoice(const AlgorithmList *list) {
    int choice;

    printf("\nVotre choix (0-%d): ", list->count);
    scanf("%d", &choice);

    while (choice < 0 || choice > list->count) {
        printf("❌ Choix invalide. Veuillez choisir entre 0 et %d: ", list->count);
        scanf("%d", &choice);
    }

    if (choice == 0) {
        return -1;
    }

    return choice - 1;
}