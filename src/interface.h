#ifndef INTERFACE_H
#define INTERFACE_H

#include <gtk/gtk.h>
#include "parser.h"
#include "algo_loader.h"
#include "Diagramme_Gantt/Diagramme.h"

#define UNIT 35  // Unité de temps pour le diagramme

// Couleurs pour les processus
typedef struct {
    const char *name;
    GdkRGBA color;
} ProcessColor;

// Structure pour stocker les données de l'application
typedef struct {
    process *processes;
    int nb_processes;
    AlgorithmList algo_list;

    // Widgets GTK
    GtkWidget *window;
    GtkWidget *algo_box;
    GtkWidget *drawing_area;
    GtkWidget *scrolled_window;
    GtkWidget *info_label;
    GtkWidget *process_table;

    // Données pour le diagramme de Gantt (utilise vos structures existantes)
    TwoInOne *current_data;  // Utilise votre structure TwoInOne
    char current_algo_name[50];
    int finish_time;
} AppData;
void update_process_table(AppData *app);
// Fonction principale pour lancer l'interface
void launch_gtk_interface(process *processes, int nb_processes,
                          AlgorithmList *algo_list, int argc, char *argv[]);

#endif // INTERFACE_H