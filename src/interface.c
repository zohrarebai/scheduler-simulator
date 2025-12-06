#include "interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Import des algorithmes
#include "algos/fifo/fifo.h"
#include "algos/RoundRobin/rr.h"
#include "algos/priority/priority.h"
#include "algos/multilevel/multilevel.h"

// Variables globales pour les couleurs
ProcessColor *process_colors = NULL;
int nb_process_colors = 0;

// ===== Fonctions utilitaires =====

GdkRGBA getProcessColor(const char *processName) {
    GdkRGBA defaultColor = {0.5, 0.5, 0.5, 1};

    for (int i = 0; i < nb_process_colors; i++) {
        if (strcmp(processName, process_colors[i].name) == 0) {
            return process_colors[i].color;
        }
    }
    return defaultColor;
}

void generateProcessColors(process *processes, int nb) {
    if (process_colors != NULL) {
        free(process_colors);
    }

    process_colors = malloc(nb * sizeof(ProcessColor));
    nb_process_colors = nb;

    for (int i = 0; i < nb; i++) {
        process_colors[i].name = processes[i].name;
        process_colors[i].color.red = (double)rand() / RAND_MAX;
        process_colors[i].color.green = (double)rand() / RAND_MAX;
        process_colors[i].color.blue = (double)rand() / RAND_MAX;
        process_colors[i].color.alpha = 1.0;
    }
}

void copyProcessArray(process *source, process *destination, int size) {
    for (int i = 0; i < size; i++) {
        strcpy(destination[i].name, source[i].name);
        destination[i].ta = source[i].ta;
        destination[i].te = source[i].te;
        destination[i].priority = source[i].priority;
    }
}

int ask_quantum_dialog(GtkWidget *parent_window, const char *algo_name) {
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *label;
    GtkWidget *entry;
    GtkWidget *hbox;
    gint result;
    int quantum = 3; // Valeur par défaut

    // Créer la boîte de dialogue
    dialog = gtk_dialog_new_with_buttons(
        "Paramètre Quantum",
        GTK_WINDOW(parent_window),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "_OK", GTK_RESPONSE_OK,
        "_Annuler", GTK_RESPONSE_CANCEL,
        NULL
    );

    gtk_window_set_default_size(GTK_WINDOW(dialog), 350, 150);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_container_set_border_width(GTK_CONTAINER(content_area), 15);

    // Créer le label
    char label_text[200];
    snprintf(label_text, sizeof(label_text),
             "Entrez le quantum pour l'algorithme %s:", algo_name);
    label = gtk_label_new(label_text);
    gtk_box_pack_start(GTK_BOX(content_area), label, FALSE, FALSE, 10);

    // Créer une boîte horizontale pour le champ de saisie
    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(content_area), hbox, FALSE, FALSE, 10);

    GtkWidget *quantum_label = gtk_label_new("Quantum:");
    gtk_box_pack_start(GTK_BOX(hbox), quantum_label, FALSE, FALSE, 5);

    // Créer le champ de saisie
    entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry), "3"); // Valeur par défaut
    gtk_entry_set_max_length(GTK_ENTRY(entry), 3);
    gtk_entry_set_width_chars(GTK_ENTRY(entry), 5);
    gtk_box_pack_start(GTK_BOX(hbox), entry, TRUE, TRUE, 5);

    // Ajouter un label d'aide
    GtkWidget *help_label = gtk_label_new("(Valeur recommandée: 2-5)");
    gtk_widget_set_opacity(help_label, 0.7);
    gtk_box_pack_start(GTK_BOX(content_area), help_label, FALSE, FALSE, 5);

    gtk_widget_show_all(dialog);

    // Afficher la boîte de dialogue et attendre la réponse
    result = gtk_dialog_run(GTK_DIALOG(dialog));

    if (result == GTK_RESPONSE_OK) {
        const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
        quantum = atoi(text);

        // Validation : quantum doit être > 0
        if (quantum <= 0) {
            quantum = 3; // Retour à la valeur par défaut si invalide
            printf("⚠️  Quantum invalide, utilisation de la valeur par défaut: 3\n");
        } else {
            printf("✅ Quantum défini: %d\n", quantum);
        }
    } else {
        printf("ℹ️  Annulé, utilisation du quantum par défaut: 3\n");
    }

    gtk_widget_destroy(dialog);
    return quantum;
}

// ===== Fonction de dessin du diagramme de Gantt (basée sur votre code) =====

gboolean draw_gantt_diagram(GtkWidget *widget, cairo_t *cr, gpointer data) {
    printf("\n=== DEBUT draw_gantt_diagram ===\n");
    AppData *app = (AppData *)data;
    printf("1. app = %p\n", (void*)app);

    if (app->current_data == NULL || app->current_data->list == NULL) {
        printf("2. ERREUR: current_data ou list est NULL\n");
        // Afficher un message si aucun algorithme n'est sélectionné
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_font_size(cr, 20);
        cairo_move_to(cr, 100, 100);
        cairo_show_text(cr, "Sélectionnez un algorithme pour afficher le diagramme");
        return FALSE;
    }
    printf("3. OK: Données présentes\n");

    listHistorics *processes = app->current_data->list;
    listQueueState *readyQueue = app->current_data->queueList;

    printf("4. processes = %p, readyQueue = %p\n", (void*)processes, (void*)readyQueue);

    int marge = 50;
    int count = 0;
    int queueCount = 0;

    // Dessiner l'axe du temps
    cairo_set_font_size(cr, 15);
    gdk_cairo_set_source_rgba(cr, &(GdkRGBA){0, 0, 0, 1});

    cairo_move_to(cr, marge, 140);
    cairo_show_text(cr, "0");
    cairo_move_to(cr, marge, 120);
    cairo_line_to(cr, (UNIT * app->finish_time) + marge, 120);
    cairo_stroke(cr);
    printf("5. Début traitement des processus\n");

    // Dessiner les processus en exécution (adapté de votre code)
    listHistorics *current = processes;
    while (current != NULL) {
        printf("6. Traitement processus %d: %s (readyQueueOrRunning=%d, enterOrExit=%d)\n",
               count++, current->val.nameP, current->val.readyQueueOrRunning, current->val.enterOrExit);
        historicsToDraw process = current->val;

        // Si le processus entre en exécution
        if (process.readyQueueOrRunning == 1 && process.enterOrExit == 0) {
            printf("7. Processus %s entre en exécution à t=%d\n", process.nameP, process.time);
            gint rectStart = process.time;
            gint rectWidth = 0;

            // Chercher la fin de l'exécution
            listHistorics *search = current;
            while (search != NULL) {
                if (strcmp(search->val.nameP, current->val.nameP) == 0 &&
                    ((search->val.enterOrExit == 1 && search->val.readyQueueOrRunning == 1) ||
                     (search->val.enterOrExit == 2 && search->val.readyQueueOrRunning == 1))) {
                    rectWidth = (search->val.time - current->val.time) * UNIT;
                    printf("8. Fin trouvée pour %s à t=%d (width=%d)\n",
                           current->val.nameP, search->val.time, rectWidth);
                    break;
                }
                search = search->next;
            }
            printf("9. search = %p\n", (void*)search);

            if (search != NULL) {
                printf("10. Dessin du rectangle pour %s\n", current->val.nameP);
                // Dessiner les labels de temps
                gdk_cairo_set_source_rgba(cr, &(GdkRGBA){0, 0, 0, 1});

                cairo_move_to(cr, (process.time * UNIT) + 50, 140);
                char labelStart[10];
                sprintf(labelStart, "%d", process.time);
                cairo_show_text(cr, labelStart);

                cairo_move_to(cr, (search->val.time * UNIT) + 50, 140);
                char labelEnd[10];
                sprintf(labelEnd, "%d", search->val.time);
                cairo_show_text(cr, labelEnd);

                // Dessiner le rectangle du processus
                GdkRGBA color = getProcessColor(process.nameP);
                gdk_cairo_set_source_rgba(cr, &color);

                cairo_rectangle(cr, (process.time * UNIT) + 50, 50, rectWidth, 50);
                cairo_fill(cr);

                // Dessiner le nom du processus
                gdk_cairo_set_source_rgba(cr, &(GdkRGBA){1, 1, 1, 1});
                cairo_move_to(cr, (process.time * UNIT) + 50 + 5, 80);
                cairo_show_text(cr, current->val.nameP);

                printf("11. Rectangle dessiné avec succès\n");
            } else {
                printf("12. ATTENTION: search est NULL pour %s\n", current->val.nameP);
            }
        }

        current = current->next;
    }
    printf("13. Fin traitement processus, début traitement file d'attente\n");

    // Dessiner la file d'attente (adapté de votre code)
    listQueueState *temp = readyQueue;
    printf("14. readyQueue = %p\n", (void*)temp);
    gdk_cairo_set_source_rgba(cr, &(GdkRGBA){0, 0, 0, 1});

    while (temp != NULL) {
        printf("15. Queue %d: time=%d, names='%s'\n",
               queueCount++, temp->val.time, temp->val.namesOfProcessess);
        if (strcmp(temp->val.namesOfProcessess, "")) {
            printf("16. Queue non vide, dessin...\n");
            char inputString[100];
            strcpy(inputString, temp->val.namesOfProcessess);

            char *token;
            char *tokensArray[50];
            int index = 0;

            token = strtok(inputString, ",");
            while (token != NULL && index < 50) {
                tokensArray[index++] = token;
                token = strtok(NULL, ",");
            }

            for (int i = 0; i < index; i++) {
                cairo_move_to(cr, (temp->val.time * UNIT) + 50, 180 + 20 * i);
                cairo_show_text(cr, tokensArray[i]);
            }

            cairo_move_to(cr, (temp->val.time * UNIT) + 50, 140);
            char labelStart[10];
            sprintf(labelStart, "%d", temp->val.time);
            cairo_show_text(cr, labelStart);
            printf("17. Queue dessinée\n");
        } else {
            printf("18. Queue vide, skip\n");
        }

        printf("19. Passage à la queue suivante...\n");
        temp = temp->next;
        printf("20. Nouvelle queue: %p\n", (void*)temp);
    }

    printf("21. Fin traitement queue\n");

    // Afficher le nom de l'algorithme
    cairo_set_font_size(cr, 30);
    gdk_cairo_set_source_rgba(cr, &(GdkRGBA){0, 0, 0, 1});
    cairo_move_to(cr, 100, 350);
    cairo_show_text(cr, app->current_algo_name);

    printf("22. FIN draw_gantt_diagram (succès)\n\n");
    return FALSE;
}

// ===== Exécution des algorithmes =====

void execute_algorithm(AppData *app, int algo_index) {
    // Nettoyer les données précédentes
    if (app->current_data != NULL) {
        if (app->current_data->list != NULL) {
            // Libérer la liste
            listHistorics *current = app->current_data->list;
            while (current != NULL) {
                listHistorics *temp = current;
                current = current->next;
                free(temp);
            }
        }
        if (app->current_data->queueList != NULL) {
            // Libérer la queue
            listQueueState *current = app->current_data->queueList;
            while (current != NULL) {
                listQueueState *temp = current;
                current = current->next;
                free(temp);
            }
        }
        free(app->current_data);
    }

    // Copier le tableau de processus
    process *temp_processes = malloc(app->nb_processes * sizeof(process));
    process *temp_processes2 = malloc(app->nb_processes * sizeof(process));
    copyProcessArray(app->processes, temp_processes, app->nb_processes);
    copyProcessArray(app->processes, temp_processes2, app->nb_processes);

    // Créer la structure TwoInOne
    app->current_data = malloc(sizeof(TwoInOne));
    app->current_data->list = NULL;
    app->current_data->queueList = NULL;

    // Exécuter l'algorithme sélectionné
    const char *algo_name = app->algo_list.algos[algo_index].name;
    int quantum = 3; // Valeur par défaut

    if (strcmp(algo_name, "fifo") == 0) {
        strcpy(app->current_algo_name, "FIFO");
        app->current_data->list = fifoX(temp_processes, app->nb_processes);
    }
    else if (strcmp(algo_name, "rr") == 0 || strcmp(algo_name, "RoundRobin") == 0) {
        strcpy(app->current_algo_name, "RoundRobin");
        // 🆕 Demander le quantum à l'utilisateur
        quantum = ask_quantum_dialog(app->window, "RoundRobin");
        printf("🔄 Exécution de RoundRobin avec quantum = %d\n", quantum);
        app->current_data->list = roundRobinX(temp_processes, app->nb_processes, quantum);
    }
    else if (strcmp(algo_name, "priority") == 0) {
        strcpy(app->current_algo_name, "Priority");
        app->current_data->list = priorityX(temp_processes, app->nb_processes);
    }
    else if (strcmp(algo_name, "multilevel") == 0 || strcmp(algo_name, "Multilevel") == 0) {
        strcpy(app->current_algo_name, "Multilevel");
        // 🆕 Demander le quantum à l'utilisateur
        quantum = ask_quantum_dialog(app->window, "Multilevel");
        printf("📊 Exécution de Multilevel avec quantum = %d\n", quantum);
        app->current_data->list = multilevelX(temp_processes, app->nb_processes, quantum);
    }
    else {
        printf("⚠️  Algorithme '%s' non implémenté dans l'interface\n", algo_name);
        strcpy(app->current_algo_name, "Algorithme non implémenté");
        app->current_data->list = NULL;
        free(temp_processes);
        free(temp_processes2);
        return;
    }

    // Afficher la liste (debug)
    if (app->current_data->list != NULL) {
        printList(app->current_data->list);
    }

    // Calculer le temps de fin
    app->finish_time = 0;
    listHistorics *current = app->current_data->list;
    while (current != NULL) {
        if (current->val.time > app->finish_time) {
            app->finish_time = current->val.time;
        }
        current = current->next;
    }

    // Générer l'état de la file d'attente (utilise votre fonction)
    app->current_data->queueList = setWhoIsInTheQueue(app->current_data->list,
                                                        app->nb_processes,
                                                        temp_processes2);

    free(temp_processes);
    free(temp_processes2);

    // Mettre à jour la taille de la zone de dessin
    gtk_widget_set_size_request(app->drawing_area,
                                100 + (app->finish_time * UNIT), 600);

    // Forcer le redessin
    gtk_widget_queue_draw(app->drawing_area);

    // Mettre à jour le label d'info
    char info_text[200];
    if (strcmp(algo_name, "rr") == 0 || strcmp(algo_name, "RoundRobin") == 0 ||
        strcmp(algo_name, "multilevel") == 0 || strcmp(algo_name, "Multilevel") == 0) {
        sprintf(info_text, "📊 Algorithme: %s (Quantum: %d) | ⏱️ Temps total: %d | 💻 Processus: %d",
                app->current_algo_name, quantum, app->finish_time, app->nb_processes);
        } else {
            sprintf(info_text, "📊 Algorithme: %s | ⏱️ Temps total: %d | 💻 Processus: %d",
                    app->current_algo_name, app->finish_time, app->nb_processes);
        }
    gtk_label_set_text(GTK_LABEL(app->info_label), info_text);
}

// ===== Callback pour les boutons d'algorithmes =====

void on_algorithm_button_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    int algo_index = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(widget), "algo_index"));

    printf("🚀 Exécution de l'algorithme: %s\n", app->algo_list.algos[algo_index].display_name);
    execute_algorithm(app, algo_index);
}

// ===== Création de l'interface =====

void create_interface(AppData *app) {
    // Fenêtre principale
    app->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(app->window), "Simulateur d'Ordonnancement");
    gtk_window_set_default_size(GTK_WINDOW(app->window), 1400, 800);
    g_signal_connect(app->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Conteneur vertical principal
    GtkWidget *main_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(main_vbox), 15);
    gtk_container_add(GTK_CONTAINER(app->window), main_vbox);

    // Titre
    GtkWidget *title_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title_label),
                         "<span font='22' weight='bold'>🖥️ Simulateur d'Ordonnancement de Processus</span>");
    gtk_box_pack_start(GTK_BOX(main_vbox), title_label, FALSE, FALSE, 10);

    // Separator
    GtkWidget *separator1 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(main_vbox), separator1, FALSE, FALSE, 5);

    // Label pour les algorithmes
    GtkWidget *algo_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(algo_label),
                         "<span font='14' weight='bold'>📚 Algorithmes d'ordonnancement disponibles:</span>");
    gtk_widget_set_halign(algo_label, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(main_vbox), algo_label, FALSE, FALSE, 5);

    // Boîte pour les boutons d'algorithmes
    app->algo_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(main_vbox), app->algo_box, FALSE, FALSE, 5);

    // Créer les boutons pour chaque algorithme
    for (int i = 0; i < app->algo_list.count; i++) {
        GtkWidget *button = gtk_button_new_with_label(app->algo_list.algos[i].display_name);

        // Style CSS pour les boutons
        GtkStyleContext *context = gtk_widget_get_style_context(button);
        gtk_style_context_add_class(context, "suggested-action");

        g_object_set_data(G_OBJECT(button), "algo_index", GINT_TO_POINTER(i));
        g_signal_connect(button, "clicked", G_CALLBACK(on_algorithm_button_clicked), app);
        gtk_box_pack_start(GTK_BOX(app->algo_box), button, FALSE, FALSE, 0);
    }

    // Separator
    GtkWidget *separator2 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(main_vbox), separator2, FALSE, FALSE, 5);

    // Label d'information
    app->info_label = gtk_label_new("👆 Sélectionnez un algorithme pour afficher le diagramme de Gantt");
    gtk_widget_set_halign(app->info_label, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(main_vbox), app->info_label, FALSE, FALSE, 5);

    // Frame pour le diagramme
    GtkWidget *frame = gtk_frame_new("📊 Diagramme de Gantt");
    gtk_box_pack_start(GTK_BOX(main_vbox), frame, TRUE, TRUE, 5);

    // Zone de dessin avec scroll
    app->scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(app->scrolled_window),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(frame), app->scrolled_window);

    app->drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(app->drawing_area, 1200, 500);
    gtk_container_add(GTK_CONTAINER(app->scrolled_window), app->drawing_area);
    g_signal_connect(app->drawing_area, "draw", G_CALLBACK(draw_gantt_diagram), app);

    gtk_widget_show_all(app->window);
}

// ===== Fonction principale pour lancer l'interface =====

void launch_gtk_interface(process *processes, int nb_processes,
                          AlgorithmList *algo_list, int argc, char *argv[]) {
    // Initialiser GTK
    gtk_init(&argc, &argv);

    // Créer la structure de données de l'application
    AppData *app = malloc(sizeof(AppData));
    app->processes = processes;
    app->nb_processes = nb_processes;
    app->algo_list = *algo_list;
    app->current_data = NULL;
    app->finish_time = 0;
    strcpy(app->current_algo_name, "");

    // Générer les couleurs pour les processus
    generateProcessColors(app->processes, app->nb_processes);

    // Créer l'interface
    create_interface(app);

    // Exécuter FIFO par défaut
    printf("🎯 Exécution de l'algorithme par défaut: FIFO\n");
    int fifo_index = -1;
    for (int i = 0; i < app->algo_list.count; i++) {
        if (strcmp(app->algo_list.algos[i].name, "fifo") == 0 ||
            strcmp(app->algo_list.algos[i].name, "Fifo") == 0) {
            fifo_index = i;
            break;
            }
    }

    if (fifo_index != -1) {
        execute_algorithm(app, fifo_index);
    } else {
        // Si FIFO n'est pas trouvé, exécuter le premier algorithme disponible
        if (app->algo_list.count > 0) {
            printf("⚠️  FIFO non trouvé, exécution du premier algorithme disponible\n");
            execute_algorithm(app, 0);
        }
    }

    // Lancer la boucle GTK
    gtk_main();

    // Nettoyage
    if (app->current_data != NULL) {
        if (app->current_data->list != NULL) {
            listHistorics *current = app->current_data->list;
            while (current != NULL) {
                listHistorics *temp = current;
                current = current->next;
                free(temp);
            }
        }
        if (app->current_data->queueList != NULL) {
            listQueueState *current = app->current_data->queueList;
            while (current != NULL) {
                listQueueState *temp = current;
                current = current->next;
                free(temp);
            }
        }
        free(app->current_data);
    }

    if (process_colors != NULL) {
        free(process_colors);
    }
    free(app);
}