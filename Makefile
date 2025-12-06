# Nom de l'exécutable
EXEC = simulateur

# Compilateur
CC = gcc
CFLAGS = -Wall -g `pkg-config --cflags gtk+-3.0`
LIBS = `pkg-config --libs gtk+-3.0`

# Dossiers
SRCDIR = src
DIAGDIR = $(SRCDIR)/Diagramme_Gantt
ALGODIR = $(SRCDIR)/algos

# Sources
SRC = $(SRCDIR)/main.c \
      $(SRCDIR)/parser.c \
      $(SRCDIR)/algo_loader.c \
      $(SRCDIR)/interface.c \
      $(DIAGDIR)/Diagramme.c \
      $(ALGODIR)/fifo/fifo.c \
      $(ALGODIR)/multilevel/multilevel.c \
      $(ALGODIR)/priority/priority.c \
      $(ALGODIR)/RoundRobin/rr.c \
      $(SRCDIR)/queue/queue.c 


# Objets
OBJ = $(SRC:.c=.o)

# Règle par défaut
all: $(EXEC)

# Compilation de l'exécutable
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC) $(LIBS)

# Compilation des fichiers .c en .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -f $(OBJ) $(EXEC)

# Lancer le simulateur
run: $(EXEC)
	./$(EXEC) config/process.txt
