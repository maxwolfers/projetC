#ifndef LOGIQUE_H
#define LOGIQUE_H

#include <time.h>

#define N 10

typedef enum {
    CMD_NONE,
    CMD_UP,
    CMD_DOWN,
    CMD_LEFT,
    CMD_RIGHT,
    CMD_SELECT,
    CMD_QUIT,
    CMD_SAVE
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
    int vies;
} Contrat;

void initJeu(char (*mat)[N], Contrat *c, int niveau);
void switchPos(char (*mat)[N], int i, int j, int k, int l);
void stabiliserPlateau(char (*mat)[N], Contrat *c, int compterPoints);
int sauvegarderPartie(char (*mat)[N], Contrat *c);
int chargerPartie(char (*mat)[N], Contrat *c);

#endif