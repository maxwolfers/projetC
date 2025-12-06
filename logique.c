#include <stdlib.h>
#include <stdio.h> 
#include "logique.h"

const char CHOICES[] = {'S', 'F', 'P', 'O', 'M'};
const int NB_CHOICES = 5;

int getHauteurVerticale(char (*mat)[N], int i, int j, int *top, int *bottom) {
    char type = mat[i][j];
    *top = i; *bottom = i;
    while (*top > 0 && mat[*top - 1][j] == type) (*top)--;
    while (*bottom < N - 1 && mat[*bottom + 1][j] == type) (*bottom)++;
    return (*bottom - *top + 1);
}

void shuffle(char *array, int n) {
    for (int i = 0; i < n - 1; i++) {
        int j = i + rand() % (n - i);
        char t = array[j]; array[j] = array[i]; array[i] = t;
    }
}

void updateContrat(Contrat *c, char item) {
    c->score++;
    for(int i=0; i<3; i++) {
        if(c->missions[i].type == item && c->missions[i].mange < c->missions[i].aManger) {
            c->missions[i].mange++;
        }
    }
}

int marquerAlignements(char (*mat)[N], int (*aSupprimer)[N]) {
    int trouve = 0;
    for(int i=0; i<N; i++) for(int j=0; j<N; j++) aSupprimer[i][j] = 0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N - 2; j++) {
            char type = mat[i][j];
            if (mat[i][j+1] == type && mat[i][j+2] == type) {
                int topG, botG, topD, botD;
                int hG = getHauteurVerticale(mat, i, j, &topG, &botG);
                int hD = getHauteurVerticale(mat, i, j+2, &topD, &botD);
                if (hG >= 3 && hD >= 3) {
                    aSupprimer[i][j+1] = 1;
                    for (int k = topG; k <= botG; k++) aSupprimer[k][j] = 1;
                    for (int k = topD; k <= botD; k++) aSupprimer[k][j+2] = 1;
                    trouve = 1;
                }
            }
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N - 2; j++) {
            if (mat[i][j] == mat[i][j+1] && mat[i][j] == mat[i][j+2]) {
                aSupprimer[i][j] = 1; aSupprimer[i][j+1] = 1; aSupprimer[i][j+2] = 1; trouve = 1;
            }
        }
    }
    for (int i = 0; i < N - 2; i++) {
        for (int j = 0; j < N; j++) {
            if (mat[i][j] == mat[i+1][j] && mat[i][j] == mat[i+2][j]) {
                aSupprimer[i][j] = 1; aSupprimer[i+1][j] = 1; aSupprimer[i+2][j] = 1; trouve = 1;
            }
        }
    }
    return trouve;
}

void appliquerGravite(char (*mat)[N], int (*aSupprimer)[N], Contrat *c, int updateScore) {
    if(updateScore) {
        for(int i=0; i<N; i++) for(int j=0; j<N; j++) if(aSupprimer[i][j]) updateContrat(c, mat[i][j]);
    }
    for (int j = 0; j < N; j++) {
        int writePos = N - 1;
        for (int i = N - 1; i >= 0; i--) {
            if (aSupprimer[i][j] == 0) { mat[writePos][j] = mat[i][j]; writePos--; }
        }
        for (int i = writePos; i >= 0; i--) mat[i][j] = CHOICES[rand() % NB_CHOICES];
    }
}

void initJeu(char (*mat)[N], Contrat *c, int niveau) {
    char tempChoices[5];
    for(int i=0; i<5; i++) tempChoices[i] = CHOICES[i];
    shuffle(tempChoices, 5);

    // On ne reset le score et les vies que si c'est le niveau 1 (début de partie)
    if (niveau == 1) {
        c->score = 0;
        c->vies = 5; // On commence avec 5 vies
    }

    c->coups = 0;
    c->debut = time(NULL);
    c->niveau = niveau;

    if (niveau == 1) {      c->maxCoups = 15; c->missions[0].aManger = 10; c->missions[1].aManger = 5; c->missions[2].aManger = 3; }
    else if (niveau == 2) { c->maxCoups = 20; c->missions[0].aManger = 15; c->missions[1].aManger = 10; c->missions[2].aManger = 5; }
    else {                  c->maxCoups = 25; c->missions[0].aManger = 20; c->missions[1].aManger = 15; c->missions[2].aManger = 10; }

    // Init missions
    c->missions[0].type = tempChoices[0]; c->missions[0].mange = 0;
    c->missions[1].type = tempChoices[1]; c->missions[1].mange = 0;
    c->missions[2].type = tempChoices[2]; c->missions[2].mange = 0;

    for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) mat[i][j] = CHOICES[rand() % NB_CHOICES];
}

void switchPos(char (*mat)[N], int i, int j, int k, int l) {
    char tmp = mat[i][j]; mat[i][j] = mat[k][l]; mat[k][l] = tmp;
}

void stabiliserPlateau(char (*mat)[N], Contrat *c, int compterPoints) {
    int aSupprimer[N][N];
    while (marquerAlignements(mat, aSupprimer)) appliquerGravite(mat, aSupprimer, c, compterPoints);
}

int sauvegarderPartie(char (*mat)[N], Contrat *c) {
    FILE *pf = fopen("savegame.dat", "wb");
    if (!pf) return 0;
    fwrite(mat, sizeof(char), N * N, pf);
    fwrite(c, sizeof(Contrat), 1, pf);
    fclose(pf);
    return 1;
}

int chargerPartie(char (*mat)[N], Contrat *c) {
    FILE *pf = fopen("savegame.dat", "rb");
    if (!pf) return 0;
    fread(mat, sizeof(char), N * N, pf);
    fread(c, sizeof(Contrat), 1, pf);
    c->debut = time(NULL);
    fclose(pf);
    return 1;
}