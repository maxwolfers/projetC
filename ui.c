#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include "ui.h"

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_SPACE 32
#define KEY_Q 113
#define KEY_S 115
#define KEY_M 109

void initConsole() {
    SetConsoleOutputCP(65001);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void Color(int text, int bg) {
    HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H, bg * 16 + text);
}

void gotoligcol(int lig, int col) {
    COORD mycoord;
    mycoord.X = col; mycoord.Y = lig;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), mycoord);
}

void printEmoji(char c) {
    switch(c) {
        case 'S': printf("🌞"); break;
        case 'F': printf("🍓"); break;
        case 'P': printf("🍎"); break;
        case 'O': printf("🧅"); break;
        case 'M': printf("🍊"); break;
        default:  printf("%c", c);
    }
}

void printJeu(char (*mat)[N], Contrat *c, int curLig, int curCol, int selLig, int selCol, int clear) {
    if (clear) system("cls");
    else gotoligcol(0, 0);

    time_t now = time(NULL);
    int elapsed = (int)difftime(now, c->debut);

    Color(15, 0);
    printf("\n     ");
    for(int j = 0; j < N; j++) printf(" %d  ", j + 1);
    printf("\n");

    for (int i = 0; i < N; i++) {
        Color(15, 0); printf("%2d |", i + 1);
        for (int j = 0; j < N; j++) {
            int bg = 0, txt = 15;
            if (i == selLig && j == selCol) { bg = 15; txt = 0; }
            else if (i == curLig && j == curCol) { bg = 1; txt = 15; }

            Color(txt, bg);
            printf(" "); printEmoji(mat[i][j]); printf(" ");
            Color(15, 0);
        }

        printf("        ");
        if (i == 0)      printf("=== NIVEAU %d ===", c->niveau);
        else if (i == 1) {
            printf("VIES : ");
            Color(12, 0);
            for(int v=0; v<c->vies; v++) printf("♥ ");
            Color(15, 0);
        }
        else if (i == 2) printf("SCORE : %d", c->score);
        else if (i == 3) { printf("[ "); printEmoji(c->missions[0].type); printf(" ] : %d/%d", c->missions[0].mange, c->missions[0].aManger); }
        else if (i == 4) { printf("[ "); printEmoji(c->missions[1].type); printf(" ] : %d/%d", c->missions[1].mange, c->missions[1].aManger); }
        else if (i == 5) { printf("[ "); printEmoji(c->missions[2].type); printf(" ] : %d/%d", c->missions[2].mange, c->missions[2].aManger); }

        gotoligcol(i + 2, 75);
        if (i == 0) printf("CHRONO : %02d:%02d", elapsed / 60, elapsed % 60);
        else if (i == 3) {
             if(c->coups >= c->maxCoups - 5) Color(12, 0);
             printf("COUPS : %d / %d", c->coups, c->maxCoups);
             Color(15, 0);
        }
        printf("\n");
    }
    Color(15, 0);
    // AJOUT DE [M]: Musique ICI
    printf("\n[FLECHES]: Bouger  [ESPACE]: Selectionner\n[S]: Sauvegarder   [M]: Musique   [Q]: Menu\n\n");
    for(int k=0; k<3; k++) printf("                                                                      \n");
}

void afficherMenu(int musiqueActive) {
    system("cls");
    Color(15, 0);
    printf("\n========================================\n");
    printf("      PLANTAMITZ 2025 - MENU\n");
    printf("========================================\n\n");

    printf("   1. Nouvelle partie \n");
    printf("   2. Charger une partie\n");
    printf("   3. Quitter\n\n"); // On enlève l'option musique de la liste principale pour la mettre à part

    printf("Votre choix (1-3) : ");

    // --- AFFICHAGE ETAT MUSIQUE A DROITE ---
    gotoligcol(5, 50); // Ligne 5, Colonne 50 (A droite)
    printf("--------------------------");
    gotoligcol(6, 50);
    if (musiqueActive) {
        printf("| MUSIQUE : [ON]  🔊     |");
    } else {
        printf("| MUSIQUE : [OFF] 🔇     |");
    }
    gotoligcol(7, 50);
    printf("| [M] pour changer       |");
    gotoligcol(8, 50);
    printf("--------------------------");

    gotoligcol(9, 21);
}

int toucheAppuyee() {
    return _kbhit();
}

Command recupererCommande() {
    int c = _getch();
    if (c == 224 || c == 0) {
        c = _getch();
        switch(c) {
            case KEY_UP: return CMD_UP;
            case KEY_DOWN: return CMD_DOWN;
            case KEY_LEFT: return CMD_LEFT;
            case KEY_RIGHT: return CMD_RIGHT;
        }
    }
    else if (c == KEY_SPACE) return CMD_SELECT;
    else if (c == 's' || c == 'S' || c == KEY_S) return CMD_SAVE;
    else if (c == 'q' || c == 'Q' || c == KEY_Q) return CMD_QUIT;
    else if (c == 'm' || c == 'M' || c == KEY_M) return CMD_MUTE;
    return CMD_NONE;
}