// main.c - Programme principal du simulateur d'ordonnancement

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "parser.h"

int main(void) {

    process *pTab = NULL;
    int arraySize = 0;
    int continueWithTheOldConfigFile;

    // Initialiser le générateur de nombres aléatoires
    srand(time(NULL));

    printf("╔═══════════════════════════════════════════════════╗\n");
    printf("║   Simulateur d'Ordonnancement de Processus       ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n\n");

    // ===== Choix de l'utilisateur =====
    printf("1: Continuer avec le fichier de configuration existant (process.txt)\n");
    printf("2: Générer un nouveau fichier de configuration\n");
    printf(">> ");
    scanf("%d", &continueWithTheOldConfigFile);

    // Validation du choix
    while (continueWithTheOldConfigFile > 2 || continueWithTheOldConfigFile < 1) {
        printf("Choix invalide ! Veuillez choisir 1 ou 2.\n");
        printf("1: Continuer avec le fichier existant\n");
        printf("2: Générer un nouveau fichier\n");
        printf(">> ");
        scanf("%d", &continueWithTheOldConfigFile);
    }

    // ===== Option 1: Utiliser le fichier existant =====
    if (continueWithTheOldConfigFile == 1) {

        printf("\n📂 Chargement du fichier 'process.txt'...\n");

        if (parseConfigFile("config/process.txt", &pTab, &arraySize) == -1) {
            fprintf(stderr, "\n❌ Erreur: Impossible de charger le fichier process.txt\n");
            fprintf(stderr, "Assurez-vous que le fichier existe et est bien formaté.\n");
            return EXIT_FAILURE;
        }

        printf("✅ %d processus chargés avec succès depuis config.txt\n\n", arraySize);

    }
    // ===== Option 2: Générer un nouveau fichier =====
    else if (continueWithTheOldConfigFile == 2) {

        int nbProcesses;
        int minTa, maxTa;
        int minTe, maxTe;
        int minP, maxP;

        printf("\n╔═══════════════════════════════════════════════════╗\n");
        printf("║     Génération d'un nouveau fichier process       ║\n");
        printf("╚═══════════════════════════════════════════════════╝\n");

        // Nombre de processus
        printf("\nEntrez le nombre de processus: ");
        scanf("%d", &nbProcesses);

        // Temps d'arrivée (ta)
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

        // Temps d'exécution (te)
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

        // Priorité
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

        // Génération du fichier
        printf("\n🔄 Génération du fichier process.txt...\n");

        if (generateConfigFile("config/process.txt", nbProcesses,
                               minTa, maxTa, minTe, maxTe, minP, maxP) == -1) {
            fprintf(stderr, "❌ Erreur lors de la génération du fichier\n");
            return EXIT_FAILURE;
        }

        printf("✅ Fichier process.txt généré avec succès!\n");

        // Charger le fichier nouvellement généré
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

    // ===== Section pour les algorithmes (à implémenter) =====
    printf("\n╔═══════════════════════════════════════════════════╗\n");
    printf("║      Sélection de l'algorithme d'ordonnancement   ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n");
    printf("\n[INFO] Cette section sera implémentée prochainement.\n");
    printf("Les algorithmes disponibles seront:\n");
    printf("  1. FIFO\n");
    printf("  3. Round Robin\n");
    printf("  4. Priority\n");
    printf("  5. Multilevel\n");

    // ===== Libération de la mémoire =====
    freeProcessArray(pTab);

    printf("\n✅ Mémoire libérée. Fin du programme.\n");
    printf("Au revoir! ♥\n");
    printf("Girl Team\n");

    return EXIT_SUCCESS;
}