# Makefile simplifié pour tester Round Robin
CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11
SRCDIR = src
ALGODIR = $(SRCDIR)/algos
RRDIR = $(ALGODIR)/Round\ Robin
QUEUEDIR = $(ALGODIR)/queue
HISTDIR = $(ALGODIR)/historicsToDraw

# Programme de test
TEST_RR = test_rr

# Règle par défaut
all: $(TEST_RR)

# Compilation du test Round Robin
$(TEST_RR): test_rr.c $(SRCDIR)/parser.c $(RRDIR)/rr.c $(QUEUEDIR)/queue.c $(HISTDIR)/historicsToDraw.c
	$(CC) $(CFLAGS) -o $@ $^

# Exécuter le test
run: $(TEST_RR)
	./$(TEST_RR)

# Nettoyer les fichiers compilés
clean:
	rm -f $(TEST_RR)

# Afficher l'aide
help:
	@echo "Commandes disponibles:"
	@echo "  make          - Compiler le test Round Robin"
	@echo "  make run      - Compiler et exécuter le test"
	@echo "  make clean    - Nettoyer les fichiers compilés"
	@echo "  make help     - Afficher cette aide"

.PHONY: all run clean help