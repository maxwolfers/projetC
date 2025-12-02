#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <wchar.h>

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

static void writeWide(const wchar_t *ws) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == NULL || hOut == INVALID_HANDLE_VALUE) {
        // Fallback: nothing special, ignore
        return;
    }
    DWORD written = 0;
    WriteConsoleW(hOut, ws, (DWORD)wcslen(ws), &written, NULL);
}

void printEmoji(char c) {
    switch (c) {
        case 'S': writeWide(L"☀️"); break; // Soleil
        case 'F': writeWide(L"🍓"); break; // Fraise
        case 'P': writeWide(L"🍎"); break; // Pomme
        case 'O': writeWide(L"🧅"); break; // Oignon
        case 'M': writeWide(L"🍊"); break; // Mandarine
        default:  printf("%c ", c); break;
    }
}

void printJeu(char (*mat)[N], Contrat *c) {
    printf("\n    ");
    for(int j = 0; j < N; j++) printf("%2d ", j + 1);
    printf("\n");

    for (int i = 0; i < N; i++) {
        printf("%2d | ", i + 1);
        for (int j = 0; j < N; j++) {
            // ICI : On utilise la fonction emoji au lieu de printf("%c")
            printEmoji(mat[i][j]);
            printf(" "); // Espace pour aérer car les emojis sont parfois larges
        }

        printf("        "); // Un peu moins d'espace car les emojis prennent de la place

        // Affichage du contrat à droite
        if (i == 0)      printf("=== CONTRAT ===");
        else if (i == 1) printf("SCORE : %d", c->score);
        else if (i == 2) printf("MISSIONS :");
        else if (i == 3) { printf("[ "); printEmoji(c->missions[0].type); printf("] : %d / %d", c->missions[0].mange, c->missions[0].aManger); }
        else if (i == 4) { printf("[ "); printEmoji(c->missions[1].type); printf("] : %d / %d", c->missions[1].mange, c->missions[1].aManger); }
        else if (i == 5) { printf("[ "); printEmoji(c->missions[2].type); printf("] : %d / %d", c->missions[2].mange, c->missions[2].aManger); }
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

    // On monte tant que c'est pareil et qu'on ne sort pas de la grille
    while (*top > 0 && mat[*top - 1][j] == type) {
        (*top)--;
    }

    // On descend tant que c'est pareil
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
        // On cherche un PONT horizontal de 3 items (j, j+1, j+2)
        for (int j = 0; j < N - 2; j++) {
            char type = mat[i][j];

            // Vérifie le pont : [X][X][X]
            if (mat[i][j+1] == type && mat[i][j+2] == type) {

                // Le pont existe. Vérifions les PILIERS aux extrémités (j et j+2)
                int topG, botG, topD, botD;
                int hG = getHauteurVerticale(mat, i, j, &topG, &botG);     // Pilier Gauche
                int hD = getHauteurVerticale(mat, i, j+2, &topD, &botD);   // Pilier Droit

                // Si les deux piliers font au moins 3 de haut => C'EST UN H !
                if (hG >= 3 && hD >= 3) {
                    // On marque tout : le pont + les piliers entiers
                    aSupprimer[i][j+1] = 1; // Milieu du pont

                    for (int k = topG; k <= botG; k++) aSupprimer[k][j] = 1;   // Tout le pilier gauche
                    for (int k = topD; k <= botD; k++) aSupprimer[k][j+2] = 1; // Tout le pilier droit

                    trouve = 1;
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
    SetConsoleOutputCP(65001);

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
        i--; j--; k--; l--;

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