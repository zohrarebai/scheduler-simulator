#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "algo_loader.h"
#include "interface.h"

int main(int argc, char *argv[]) {
    process *pTab = NULL;
    int arraySize = 0;
    char *config_file;

    printf("╔═══════════════════════════════════════════════════╗\n");
    printf("║   Simulateur d'Ordonnancement de Processus       ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n\n");

    // ===== Vérifier les arguments =====
    if (argc != 2) {
        fprintf(stderr, "❌ Erreur: Nombre d'arguments incorrect\n\n");
        printf("Usage: %s <fichier_config>\n", argv[0]);
        printf("Exemple: %s config/process.txt\n\n", argv[0]);
        return EXIT_FAILURE;
    }

    // ===== Récupérer le fichier passé en paramètre =====
    config_file = argv[1];
    printf("📋 Fichier de configuration: %s\n\n", config_file);

    // ===== Charger le fichier =====
    printf("📂 Chargement du fichier...\n");

    if (parseConfigFile(config_file, &pTab, &arraySize) == -1) {
        fprintf(stderr, "\n❌ Erreur: Impossible de charger le fichier '%s'\n", config_file);
        fprintf(stderr, "Vérifiez que:\n");
        fprintf(stderr, "  - Le fichier existe\n");
        fprintf(stderr, "  - Le chemin est correct\n");
        fprintf(stderr, "  - Le format est valide\n\n");
        return EXIT_FAILURE;
    }

    printf("✅ %d processus chargés avec succès\n\n", arraySize);

    // ===== Afficher le tableau des processus =====
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║            PROCESSUS CHARGÉS EN MÉMOIRE                  ║\n");
    printf("╠═══════════╦═══════════╦═══════════════╦═══════════════════╣\n");
    printf("║   Nom     ║  Arrivée  ║   Exécution   ║     Priorité      ║\n");
    printf("╠═══════════╬═══════════╬═══════════════╬═══════════════════╣\n");

    for (int i = 0; i < arraySize; i++) {
        printf("║ %-9s ║ %9d ║ %13d ║ %17d ║\n",
               pTab[i].name,
               pTab[i].ta,
               pTab[i].te,
               pTab[i].priority);
    }

    printf("╚═══════════╩═══════════╩═══════════════╩═══════════════════╝\n");
    printf("\n📊 Total: %d processus\n\n", arraySize);

    // ===== Charger les algorithmes disponibles =====
    AlgorithmList algo_list;
    printf("📚 Chargement des algorithmes...\n");

    if (loadAvailableAlgorithms("src/algos", &algo_list) <= 0) {
        fprintf(stderr, "❌ Erreur: Aucun algorithme trouvé dans le dossier algos/\n");
        freeProcessArray(pTab);
        return EXIT_FAILURE;
    }

    printf("✅ %d algorithmes chargés\n\n", algo_list.count);

    // ===== Lancer l'interface GTK =====
    printf("🚀 Lancement de l'interface graphique...\n\n");
    launch_gtk_interface(pTab, arraySize, &algo_list, argc, argv);

    // ===== Libérer la mémoire =====
    // freeProcessArray(pTab);

    printf("\n✅ Programme terminé\n");
    printf("Au revoir! ♥\n");

    return EXIT_SUCCESS;
}