#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

#define N 10

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_SPACE 32
#define KEY_Q 113

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
    int coups;
    int maxCoups;
    time_t debut;
} Contrat;

void Color(int text, int bg) {
    HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H, bg * 16 + text);
}

void gotoligcol(int lig, int col) {
    COORD mycoord;
    mycoord.X = col;
    mycoord.Y = lig;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), mycoord);
}

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

    c->coups = 0;
    c->maxCoups = 20;
    c->debut = time(NULL);

    c->missions[0].type = tempChoices[0]; c->missions[0].aManger = 10; c->missions[0].mange = 0;
    c->missions[1].type = tempChoices[1]; c->missions[1].aManger = 5;  c->missions[1].mange = 0;
    c->missions[2].type = tempChoices[2]; c->missions[2].aManger = 3;  c->missions[2].mange = 0;
}

void updateContrat(Contrat *c, char item) {
    c->score++;
    for(int i=0; i<3; i++) {
        if(c->missions[i].type == item && c->missions[i].mange < c->missions[i].aManger) {
            c->missions[i].mange++;
        }
    }
}

void printEmoji(char c) {
    switch(c) {
        case 'S': printf("☀️"); break;
        case 'F': printf("🍓"); break;
        case 'P': printf("🍎"); break;
        case 'O': printf("🧅"); break;
        case 'M': printf("🍊"); break;
        default:  printf("%c", c);
    }
}

// MODIFICATION ICI : Ajout du paramètre 'clear'
void printJeu(char (*mat)[N], Contrat *c, int curLig, int curCol, int selLig, int selCol, int clear) {

    if (clear) {
        system("cls"); // On efface tout si demandé
    } else {
        gotoligcol(0, 0); // Sinon on remonte juste
    }

    time_t now = time(NULL);
    int elapsed = (int)difftime(now, c->debut);
    int min = elapsed / 60;
    int sec = elapsed % 60;

    Color(15, 0);
    printf("\n     ");
    for(int j = 0; j < N; j++) printf(" %d  ", j + 1);
    printf("\n");

    for (int i = 0; i < N; i++) {
        Color(15, 0);
        printf("%2d |", i + 1);

        for (int j = 0; j < N; j++) {
            int bg = 0; int txt = 15;
            if (i == selLig && j == selCol) { bg = 15; txt = 0; }
            else if (i == curLig && j == curCol) { bg = 1; txt = 15; }

            Color(txt, bg);
            printf(" ");
            printEmoji(mat[i][j]);
            printf(" ");

            Color(15, 0);
        }

        printf("       ");

        if (i == 0)      printf("=== CONTRAT ===");
        else if (i == 1) printf("SCORE : %d", c->score);
        else if (i == 2) printf("MISSIONS :");
        else if (i == 3) { printf("[ "); printEmoji(c->missions[0].type); printf(" ] : %d / %d", c->missions[0].mange, c->missions[0].aManger); }
        else if (i == 4) { printf("[ "); printEmoji(c->missions[1].type); printf(" ] : %d / %d", c->missions[1].mange, c->missions[1].aManger); }
        else if (i == 5) { printf("[ "); printEmoji(c->missions[2].type); printf(" ] : %d / %d", c->missions[2].mange, c->missions[2].aManger); }
        else if (i == 6) printf("===============");

        gotoligcol(i + 2, 75);

        if (i == 0) {
            printf("CHRONO : %02d:%02d", min, sec);
        }
        else if (i == 3) {
            if(c->coups >= c->maxCoups - 5) Color(12, 0);
            else Color(15, 0);

            printf("COUPS : %d / %d", c->coups, c->maxCoups);
            Color(15, 0);
        }

        printf("\n");
    }

    Color(15, 0);
    printf("\n[FLECHES]: Bouger  \n[ESPACE]: Selectionner  \n[Q]: Quitter\n\n");
}

void switchPos(char (*mat)[N], int i, int j, int k, int l) {
    char tmp = mat[i][j];
    mat[i][j] = mat[k][l];
    mat[k][l] = tmp;
}

int getHauteurVerticale(char (*mat)[N], int i, int j, int *top, int *bottom) {
    char type = mat[i][j];
    *top = i; *bottom = i;
    while (*top > 0 && mat[*top - 1][j] == type) (*top)--;
    while (*bottom < N - 1 && mat[*bottom + 1][j] == type) (*bottom)++;
    return (*bottom - *top + 1);
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

void stabiliserPlateau(char (*mat)[N], Contrat *c, int compterPoints) {
    int aSupprimer[N][N];
    while (marquerAlignements(mat, aSupprimer)) appliquerGravite(mat, aSupprimer, c, compterPoints);
}

int main(void) {
    SetConsoleOutputCP(65001);

    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

    char (*mat)[N] = malloc(sizeof *mat * N);
    if (!mat) return 1;

    srand((unsigned)time(NULL));
    Contrat monContrat;
    initContrat(&monContrat);

    // Initial clear
    system("cls");

    for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) mat[i][j] = CHOICES[rand() % NB_CHOICES];
    stabiliserPlateau(mat, &monContrat, 0);

    int curLig = 0, curCol = 0;
    int selLig = -1, selCol = -1;
    int running = 1;

    while (running) {
        printJeu(mat, &monContrat, curLig, curCol, selLig, selCol, 1);

        if (monContrat.coups >= monContrat.maxCoups) {
            printf("\n\nPERDU ! Vous n'avez plus de coups.\n");
            system("pause");
            break;
        }

        while (!_kbhit()) {
            Sleep(100);
            // MODIFICATION ICI : On met '0' pour ne PAS effacer l'écran (éviter le clignotement du chrono)
            printJeu(mat, &monContrat, curLig, curCol, selLig, selCol, 0);
        }

        int c = _getch();

        if (c == 224 || c == 0) {
            c = _getch();
            switch(c) {
                case KEY_UP:    if(curLig > 0) curLig--; break;
                case KEY_DOWN:  if(curLig < N-1) curLig++; break;
                case KEY_LEFT:  if(curCol > 0) curCol--; break;
                case KEY_RIGHT: if(curCol < N-1) curCol++; break;
            }
        }
        else if (c == KEY_SPACE) {
            if (selLig == -1) {
                selLig = curLig; selCol = curCol;
            } else {
                if (curLig == selLig && curCol == selCol) {
                    selLig = -1; selCol = -1;
                } else {
                    switchPos(mat, selLig, selCol, curLig, curCol);
                    monContrat.coups++;
                    stabiliserPlateau(mat, &monContrat, 1);
                    selLig = -1; selCol = -1;
                }
            }
        }
        else if (c == 'q' || c == 'Q' || c == KEY_Q) {
            running = 0;
        }
    }

    free(mat);
    return 0;
}