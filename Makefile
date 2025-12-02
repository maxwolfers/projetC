# Compilateur
CC = gcc

# Options de compilation
# -Wall -Wextra : Affiche tous les avertissements (très utile)
# -g : Ajoute les infos de debug (utile pour gdb ou CLion)
CFLAGS = -Wall -Wextra -g

# --- DÉTECTION OS (Pour Windows) ---
ifeq ($(OS),Windows_NT)
    EXE = .exe
    RM = del /Q  # Commande Windows pour supprimer
else
    EXE =
    RM = rm -f   # Commande Linux/Mac pour supprimer
endif

# Nom final de ton jeu
TARGET = plantamitz$(EXE)

# --- TES FICHIERS (C'est ici qu'on adapte) ---
# Liste de tes fichiers .c
SOURCES = main.c logique.c ui.c

# Liste de tes fichiers .h (Pour que make recompile si tu changes un header)
HEADERS = logique.h ui.h

# Génération automatique de la liste des objets (.o)
OBJECTS = $(SOURCES:.c=.o)

# --- RÈGLES ---

# Règle par défaut (quand tu tapes juste "make")
all: $(TARGET)

# Édition de liens (Création de l'exécutable final)
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Compilation des fichiers objets (.c -> .o)
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers temporaires et de l'exécutable
clean:
	$(RM) *.o $(TARGET)

# Tout reconstruire de zéro
rebuild: clean all

.PHONY: all clean rebuild