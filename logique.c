#include <stdlib.h>
#include <stdio.h> // Pour NULL
#include "logique.h"
#include <errno.h>

const char CHOICES[] = {'S', 'F', 'P', 'O', 'M'};
const int NB_CHOICES = 5;

// --- FONCTIONS INTERNES (Privées) ---
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

    // Detection H (Géometrique)
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
    // Horizontal
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N - 2; j++) {
            if (mat[i][j] == mat[i][j+1] && mat[i][j] == mat[i][j+2]) {
                aSupprimer[i][j] = 1; aSupprimer[i][j+1] = 1; aSupprimer[i][j+2] = 1; trouve = 1;
            }
        }
    }
    // Vertical
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

// --- FONCTIONS PUBLIQUES ---
void initJeu(char (*mat)[N], Contrat *c) {
    char tempChoices[5];
    for(int i=0; i<5; i++) tempChoices[i] = CHOICES[i];
    shuffle(tempChoices, 5);

    c->score = 0; c->coups = 0; c->maxCoups = 20; c->debut = time(NULL);
    c->missions[0].type = tempChoices[0]; c->missions[0].aManger = 10; c->missions[0].mange = 0;
    c->missions[1].type = tempChoices[1]; c->missions[1].aManger = 5;  c->missions[1].mange = 0;
    c->missions[2].type = tempChoices[2]; c->missions[2].aManger = 3;  c->missions[2].mange = 0;

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
    // Ouverture en mode binaire écriture "wb" (Windows Binary) [cite: 186, 440]
    FILE *pf = fopen("savegame.dat", "wb");
    
    // Vérification de l'ouverture [cite: 690]
    if (pf == NULL) {
        perror("Erreur sauvegarde");
        return 0;
    }

    // 1. Écriture du contrat (Score, coups, missions...) [cite: 529]
    // On écrit 1 élément de la taille de la structure Contrat
    if (fwrite(c, sizeof(Contrat), 1, pf) != 1) {
        perror("Erreur ecriture contrat");
        fclose(pf);
        return 0;
    }

    // 2. Écriture de la matrice (Grille de jeu) [cite: 458]
    // La matrice est contiguë en mémoire (allouée dynamiquement ou statiquement)
    // On écrit N*N caractères
    if (fwrite(mat, sizeof(char) * N * N, 1, pf) != 1) {
        perror("Erreur ecriture plateau");
        fclose(pf);
        return 0;
    }

    // Fermeture propre [cite: 767]
    fclose(pf);
    return 1;
}

int chargerPartie(char (*mat)[N], Contrat *c) {
    // Ouverture en mode binaire lecture "rb" [cite: 440]
    FILE *pf = fopen("savegame.dat", "rb");
    
    if (pf == NULL) {
        // Pas d'erreur critique ici, le fichier n'existe peut-être pas encore
        return 0; 
    }

    // 1. Lecture du contrat [cite: 549]
    if (fread(c, sizeof(Contrat), 1, pf) != 1) {
        perror("Erreur lecture contrat");
        fclose(pf);
        return 0;
    }

    // 2. Lecture de la matrice
    if (fread(mat, sizeof(char) * N * N, 1, pf) != 1) {
        perror("Erreur lecture plateau");
        fclose(pf);
        return 0;
    }
    
    // Astuce : Le temps "debut" sauvegardé est une date absolue (timestamp).
    // Pour que le chrono ne saute pas à une valeur énorme, on réinitialise
    // le temps de début relatif au temps actuel moins le temps déjà joué.
    // (Non couvert explicitement par le chapitre 14 mais nécessaire pour la logique du jeu)
    // Pour l'instant, on laisse tel quel, le chrono affichera le temps réel écoulé depuis la 1ère partie.

    fclose(pf);
    return 1;
}