#ifndef LOGIQUE_H
#define LOGIQUE_H

#include <time.h>

#define N 10 // Taille grille

// Commandes simplifiées que l'UI renvoie au Main
typedef enum {
    CMD_NONE,
    CMD_UP,
    CMD_DOWN,
    CMD_LEFT,
    CMD_RIGHT,
    CMD_SELECT, // Espace
    CMD_SAVE,   // S (Nouveau !)
    CMD_QUIT    // Q
} Command;

typedef struct {
    char type;
    int aManger;
    int mange;
} Objectif;

typedef struct {
    Objectif missions[3];
    int score;
    int coups;
    int maxCoups;
    time_t debut;
    int niveau;
} Contrat;

// Prototypes
void initJeu(char (*mat)[N], Contrat *c, int niveau);
void switchPos(char (*mat)[N], int i, int j, int k, int l);
void stabiliserPlateau(char (*mat)[N], Contrat *c, int compterPoints);

// Gestion de fichiers (Basé sur Chapitre 14)
int sauvegarderPartie(char (*mat)[N], Contrat *c);
int chargerPartie(char (*mat)[N], Contrat *c);

#endif