#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <time.h>
#include "parser.h"
#include "algo_loader.h"
// Manual declaration of roundRobinX function
void* roundRobinX(process *dummy, int nb, int qu);

int main(void) {

    process *pTab = NULL;
    int arraySize = 0;
    int continueWithTheOldConfigFile;

    srand(time(NULL));

    printf("╔═══════════════════════════════════════════════════╗\n");
    printf("║   Simulateur d'Ordonnancement de Processus       ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n\n");


    printf("1: Continuer avec le fichier de configuration existant (config.txt)\n");
    printf("2: Générer un nouveau fichier de configuration\n");
    printf(">> ");
    scanf("%d", &continueWithTheOldConfigFile);


    while (continueWithTheOldConfigFile > 2 || continueWithTheOldConfigFile < 1) {
        printf("Choix invalide ! Veuillez choisir 1 ou 2.\n");
        printf("1: Continuer avec le fichier existant\n");
        printf("2: Générer un nouveau fichier\n");
        printf(">> ");
        scanf("%d", &continueWithTheOldConfigFile);
    }


    if (continueWithTheOldConfigFile == 1) {

        printf("\n📂 Chargement du fichier 'config/process.txt'...\n");

        if (parseConfigFile("config/process.txt", &pTab, &arraySize) == -1) {
            fprintf(stderr, "\n❌ Erreur: Impossible de charger le fichier config/process.txt\n");
            fprintf(stderr, "Assurez-vous que le fichier existe et est bien formaté.\n");
            return EXIT_FAILURE;
        }

        printf("✅ %d processus chargés avec succès depuis config.txt\n\n", arraySize);

    }

    else if (continueWithTheOldConfigFile == 2) {

        int nbProcesses;
        int minTa, maxTa;
        int minTe, maxTe;
        int minP, maxP;

        printf("\n╔═══════════════════════════════════════════════════╗\n");
        printf("║     Génération d'un nouveau fichier config       ║\n");
        printf("╚═══════════════════════════════════════════════════╝\n");


        printf("\nEntrez le nombre de processus: ");
        scanf("%d", &nbProcesses);

        printf("\n--- Configuration du temps d'arrivée (ta) ---\n");
        printf("Entrez le temps d'arrivée minimal: ");
        scanf("%d", &minTa);
        printf("Entrez le temps d'arrivée maximal: ");
        scanf("%d", &maxTa);

        while (maxTa < minTa) {
            printf("❌ Erreur: Le maximum doit être >= %d\n", minTa);
            printf("Entrez un nombre supérieur ou égal à %d: ", minTa);
            scanf("%d", &maxTa);
        }

        printf("\n--- Configuration du temps d'exécution (te) ---\n");
        printf("Entrez le temps d'exécution minimal: ");
        scanf("%d", &minTe);
        printf("Entrez le temps d'exécution maximal: ");
        scanf("%d", &maxTe);

        while (maxTe < minTe) {
            printf("❌ Erreur: Le maximum doit être >= %d\n", minTe);
            printf("Entrez un nombre supérieur ou égal à %d: ", minTe);
            scanf("%d", &maxTe);
        }

        printf("\n--- Configuration de la priorité ---\n");
        printf("Entrez la priorité minimale: ");
        scanf("%d", &minP);
        printf("Entrez la priorité maximale: ");
        scanf("%d", &maxP);

        while (maxP < minP) {
            printf("❌ Erreur: Le maximum doit être >= %d\n", minP);
            printf("Entrez un nombre supérieur ou égal à %d: ", minP);
            scanf("%d", &maxP);
        }

        printf("\n🔄 Génération du fichier config/process.txt...\n");

        if (generateConfigFile("config/process.txt", nbProcesses,
                               minTa, maxTa, minTe, maxTe, minP, maxP) == -1) {
            fprintf(stderr, "❌ Erreur lors de la génération du fichier\n");
            return EXIT_FAILURE;
        }

        printf("✅ Fichier config/process.txt généré avec succès!\n");

        printf("📂 Chargement du fichier généré...\n");

        if (parseConfigFile("config/process.txt", &pTab, &arraySize) == -1) {
            fprintf(stderr, "❌ Erreur lors du chargement du fichier généré\n");
            return EXIT_FAILURE;
        }

        printf("✅ %d processus chargés en mémoire\n\n", arraySize);
    }

    // ===== Affichage des processus chargés =====
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║            PROCESSUS CHARGÉS EN MÉMOIRE                  ║\n");
    printf("╠═══════════╦═══════════╦═══════════════╦═══════════════════╣\n");
    printf("║   Nom     ║  Arrivée  ║   Exécution   ║     Priorité      ║\n");
    printf("╠═══════════╬═══════════╬═══════════════╬═══════════════════╣\n");

    for (int i = 0; i < arraySize; i++) {
        printf("║ %-9s ║ %9d ║ %13d ║ %17d ║\n",
               pTab[i].name, pTab[i].ta, pTab[i].te, pTab[i].priority);
    }

    printf("╚═══════════╩═══════════╩═══════════════╩═══════════════════╝\n");
    printf("\n📊 Total: %d processus chargés\n", arraySize);

    AlgorithmList algoList;

    printf("\n🔍 Recherche des algorithmes disponibles...\n");

    int algoCount = loadAvailableAlgorithms("src/algos", &algoList);

    if (algoCount <= 0) {
        fprintf(stderr, "❌ Aucun algorithme disponible.\n");
        fprintf(stderr, "Assurez-vous que le dossier src/algos/ existe et contient des sous-dossiers.\n");
        freeProcessArray(pTab);
        return EXIT_FAILURE;
    }

    printf("✅ %d algorithme(s) détecté(s)\n", algoCount);

    displayAlgorithmMenu(&algoList);

    int selectedAlgo = getAlgorithmChoice(&algoList); //algo choisis par l'utilisateur et a faire

    if (selectedAlgo == -1) {
        printf("\n👋 Programme annulé par l'utilisateur.\n");
        freeProcessArray(pTab);
        return EXIT_SUCCESS;
    }

    printf("\n✅ Algorithme sélectionné: %s\n", algoList.algos[selectedAlgo].display_name);
    printf("📁 Dossier: src/algos/%s/\n", algoList.algos[selectedAlgo].name);

    //Exécuter l'algorithme sélectionné à faire
   
     printf("DEBUG: selectedAlgo = %d\n", selectedAlgo);
printf("DEBUG: Algorithm name = '%s'\n", algoList.algos[selectedAlgo].name);
printf("DEBUG: Algorithm display_name = '%s'\n", algoList.algos[selectedAlgo].display_name);
printf("DEBUG: Comparison result = %d\n", strcmp(algoList.algos[selectedAlgo].name, "Round Robin"));

if (strcmp(algoList.algos[selectedAlgo].name, "Round Robin") == 0) {
    int quantum = 2;
    printf("\n[INFO] Exécution de Round Robin avec quantum = %d\n", quantum);
    roundRobinX(pTab, arraySize, quantum);
} else {
    printf("\n[INFO] Algorithme %s pas encore implémenté\n", algoList.algos[selectedAlgo].name);
}
    freeProcessArray(pTab);

    printf("\n✅ Mémoire libérée. Fin du programme.\n");
    printf("Au revoir! ♥\n");
    printf("Girl Team\n");

    return EXIT_SUCCESS;
}