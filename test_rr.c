// test_rr.c - Programme de test pour Round Robin
#include <stdio.h>
#include <stdlib.h>
#include "src/parser.h"
#include "src/algos/Round Robin/rr.h"

int main(void) {
    process *pTab = NULL;
    int arraySize = 0;

    printf("╔═══════════════════════════════════════════════════╗\n");
    printf("║      TEST DE L'ALGORITHME ROUND ROBIN            ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n\n");

    // Charger les processus depuis le fichier
    printf("📂 Chargement du fichier 'process.txt'...\n");

    if (parseConfigFile("config/process.txt", &pTab, &arraySize) == -1) {
        fprintf(stderr, "❌ Erreur lors du chargement du fichier\n");
        return EXIT_FAILURE;
    }

    printf("✅ %d processus chargés avec succès\n\n", arraySize);

    // Afficher les processus
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║            PROCESSUS À ORDONNANCER                        ║\n");
    printf("╠═══════════╦═══════════╦═══════════════╦═══════════════════╣\n");
    printf("║   Nom     ║  Arrivée  ║   Exécution   ║     Priorité      ║\n");
    printf("╠═══════════╬═══════════╬═══════════════╬═══════════════════╣\n");

    for (int i = 0; i < arraySize; i++) {
        printf("║ %-9s ║ %9d ║ %13d ║ %17d ║\n",
               pTab[i].name, pTab[i].ta, pTab[i].te, pTab[i].priority);
    }
    printf("╚═══════════╩═══════════╩═══════════════╩═══════════════════╝\n\n");

    // Demander le quantum
    int quantum;
    printf("Entrez le quantum de temps (recommandé: 2-4): ");
    scanf("%d", &quantum);

    while (quantum <= 0) {
        printf("❌ Le quantum doit être > 0. Réessayez: ");
        scanf("%d", &quantum);
    }

    printf("\n🚀 Démarrage de l'algorithme Round Robin...\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");

    // Exécuter Round Robin (utilise la fonction de l'année dernière)
    listHistorics *historics = roundRobinX(pTab, arraySize, quantum);

    // Afficher l'historique complet
    printf("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printHistorics(historics);

    // Libérer la mémoire
    freeHistorics(historics);
    freeProcessArray(pTab);

    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("✅ Programme terminé avec succès!\n");
    printf("💜 Développé par Nada - Girl Team\n\n");

    return EXIT_SUCCESS;
}