# Makefile pour FarmHeroes
# Compilateur
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra -std=c99 -g

# Nom de l'exécutable
TARGET = farmheroes

# Fichiers sources
SOURCES = main.c game.c board.c groups.c contract.c display.c input.c save.c

# Fichiers objets
OBJECTS = $(SOURCES:.c=.o)

# Fichiers headers
HEADERS = config.h game.h board.h groups.h contract.h display.h input.h save.h

# Règle par défaut
all: $(TARGET)

# Compilation de l'exécutable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Règle générique pour les fichiers objets
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -f $(OBJECTS) $(TARGET) savegame.dat

# Réinstallation complète
rebuild: clean all

# Installation (optionnel)
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

.PHONY: all clean rebuild install

