# Compilateur
CC = gcc

# Options de compilation
# -Wall -Wextra : Avertissements
# -g : Debug
CFLAGS = -Wall -Wextra -g

# --- BIBLIOTHEQUES (NOUVEAU) ---
# On ajoute la bibliothèque multimédia de Windows pour le son (PlaySound)
# Cette variable sera vide sur Linux/Mac, et contiendra -lwinmm sur Windows
ifeq ($(OS),Windows_NT)
    EXE = .exe
    RM = del /Q
    LIBS = -lwinmm
else
    EXE =
    RM = rm -f
    LIBS =
endif

# Nom final de ton jeu
TARGET = plantamitz$(EXE)

# --- FICHIERS ---
SOURCES = main.c logique.c ui.c audio.c
HEADERS = logique.h ui.h audio.h

# Génération automatique de la liste des objets (.o)
OBJECTS = $(SOURCES:.c=.o)

# --- RÈGLES ---

all: $(TARGET)

# Édition de liens (Création de l'exécutable final)
# AJOUT DE $(LIBS) A LA FIN DE CETTE LIGNE
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

# Compilation des fichiers objets (.c -> .o)
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o $(TARGET)

rebuild: clean all

.PHONY: all clean rebuild