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
} Contrat;

// Prototypes
void initJeu(char (*mat)[N], Contrat *c);
void switchPos(char (*mat)[N], int i, int j, int k, int l);
void stabiliserPlateau(char (*mat)[N], Contrat *c, int compterPoints);

#endif