#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10

const char CHOICES[] = {'S', 'F', 'P', 'O', 'M'};
const int NB_CHOICES = 5;

typedef struct {
    char type;
    int aManger;
    int mange;
} Objectif;

typedef struct {
    Objectif missions[3];
    int score;
} Contrat;

void shuffle(char *array, int n) {
    for (int i = 0; i < n - 1; i++) {
        int j = i + rand() % (n - i);
        char t = array[j];
        array[j] = array[i];
        array[i] = t;
    }
}

void initContrat(Contrat *c) {
    char tempChoices[5];
    for(int i=0; i<5; i++) tempChoices[i] = CHOICES[i];

    shuffle(tempChoices, 5);

    c->score = 0;

    c->missions[0].type = tempChoices[0];
    c->missions[0].aManger = 10;
    c->missions[0].mange = 0;

    c->missions[1].type = tempChoices[1];
    c->missions[1].aManger = 5;
    c->missions[1].mange = 0;

    c->missions[2].type = tempChoices[2];
    c->missions[2].aManger = 3;
    c->missions[2].mange = 0;
}

void updateContrat(Contrat *c, char item) {
    c->score++;
    for(int i=0; i<3; i++) {
        if(c->missions[i].type == item) {
            if(c->missions[i].mange < c->missions[i].aManger) {
                c->missions[i].mange++;
            }
        }
    }
}

void printJeu(char (*mat)[N], Contrat *c) {
    printf("\n    ");
    for(int j = 0; j < N; j++) printf("%2d ", j + 1);
    printf("\n");

    for (int i = 0; i < N; i++) {
        printf("%2d | ", i + 1);
        for (int j = 0; j < N; j++) {
            printf("%c  ", mat[i][j]);
        }

        printf("          ");

        if (i == 0)      printf("=== CONTRAT ===");
        else if (i == 1) printf("SCORE : %d", c->score);
        else if (i == 2) printf("MISSIONS :");
        else if (i == 3) printf("[ %c ] : %d / %d", c->missions[0].type, c->missions[0].mange, c->missions[0].aManger);
        else if (i == 4) printf("[ %c ] : %d / %d", c->missions[1].type, c->missions[1].mange, c->missions[1].aManger);
        else if (i == 5) printf("[ %c ] : %d / %d", c->missions[2].type, c->missions[2].mange, c->missions[2].aManger);
        else if (i == 6) printf("===============");

        printf("\n");
    }
    printf("\n");
}

void switchPos(char (*mat)[N], int i, int j, int k, int l) {
    char tmp = mat[i][j];
    mat[i][j] = mat[k][l];
    mat[k][l] = tmp;
}

int getHauteurVerticale(char (*mat)[N], int i, int j, int *top, int *bottom) {
    char type = mat[i][j];
    *top = i;
    *bottom = i;

    // On monte tant que c'est le même type
    while (*top > 0 && mat[*top - 1][j] == type) {
        (*top)--;
    }

    // On descend tant que c'est le même type
    while (*bottom < N - 1 && mat[*bottom + 1][j] == type) {
        (*bottom)++;
    }

    return (*bottom - *top + 1); // Hauteur totale
}

int marquerAlignements(char (*mat)[N], int (*aSupprimer)[N]) {
    int trouve = 0;
    for(int i=0; i<N; i++)
        for(int j=0; j<N; j++)
            aSupprimer[i][j] = 0;

    // forme H
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N - 3; j++) {
            if (mat[i][j] == mat[i][j+1] && mat[i][j] == mat[i][j+2] && mat[i][j] == mat[i][j+3]) {
                for (int k = 0; k < N-2; k++) {
                    if (mat[k][j] == mat[k+1][j] && mat[k][j] == mat[k+2][j]) {
                        aSupprimer[i][j] = 1;
                        aSupprimer[i][j+1] = 1;
                        aSupprimer[i][j+2] = 1;
                        aSupprimer[k][j] = 1;
                        aSupprimer[k+1][j] = 1;
                        aSupprimer[k+2][j] = 1;
                        trouve = 1;
                    }
                }
            }
        }
    }

    // Horizontal
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N - 2; j++) {
            if (mat[i][j] == mat[i][j+1] && mat[i][j] == mat[i][j+2]) {
                aSupprimer[i][j] = 1;
                aSupprimer[i][j+1] = 1;
                aSupprimer[i][j+2] = 1;
                trouve = 1;
            }
        }
    }

    // Vertical
    for (int i = 0; i < N - 2; i++) {
        for (int j = 0; j < N; j++) {
            if (mat[i][j] == mat[i+1][j] && mat[i][j] == mat[i+2][j]) {
                aSupprimer[i][j] = 1;
                aSupprimer[i+1][j] = 1;
                aSupprimer[i+2][j] = 1;
                trouve = 1;
            }
        }
    }
    return trouve;
}

void appliquerGravite(char (*mat)[N], int (*aSupprimer)[N], Contrat *c, int updateScore) {
    if(updateScore) {
        for(int i=0; i<N; i++) {
            for(int j=0; j<N; j++) {
                if(aSupprimer[i][j]) {
                    updateContrat(c, mat[i][j]);
                }
            }
        }
    }

    for (int j = 0; j < N; j++) {
        int writePos = N - 1;
        for (int i = N - 1; i >= 0; i--) {
            if (aSupprimer[i][j] == 0) {
                mat[writePos][j] = mat[i][j];
                writePos--;
            }
        }
        for (int i = writePos; i >= 0; i--) {
            mat[i][j] = CHOICES[rand() % NB_CHOICES];
        }
    }
}

void stabiliserPlateau(char (*mat)[N], Contrat *c, int compterPoints) {
    int aSupprimer[N][N];
    while (marquerAlignements(mat, aSupprimer)) {
        appliquerGravite(mat, aSupprimer, c, compterPoints);
    }
}

int main(void) {
    char (*mat)[N] = malloc(sizeof *mat * N);
    if (!mat) return 1;

    srand((unsigned)time(NULL));

    Contrat monContrat;
    initContrat(&monContrat);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            mat[i][j] = CHOICES[rand() % NB_CHOICES];
        }
    }

    stabiliserPlateau(mat, &monContrat, 0);
    printJeu(mat, &monContrat);

    while (1) {
        int i, j, k, l;
        printf("Coord (L1 C1 L2 C2) ou 'q' pour quitter: ");

        if (scanf("%d %d %d %d", &i, &j, &k, &l) != 4) break;

        if (i<0||i>=N||j<0||j>=N||k<0||k>=N||l<0||l>=N) {
            continue;
        }

        switchPos(mat, i, j, k, l);
        stabiliserPlateau(mat, &monContrat, 1);
        printJeu(mat, &monContrat);
    }

    free(mat);
    return 0;
}