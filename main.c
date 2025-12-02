#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "logique.h"
#include "ui.h"

void jouerPartie() {
    char (*mat)[N] = malloc(sizeof *mat * N);
    if (!mat) return;

    Contrat monContrat;
    initJeu(mat, &monContrat);
    stabiliserPlateau(mat, &monContrat, 0);

    system("cls"); // Nettoyage initial

    int curLig = 0, curCol = 0, selLig = -1, selCol = -1;
    int running = 1;

    printJeu(mat, &monContrat, curLig, curCol, selLig, selCol, 1);

    while (running) {
        if (monContrat.coups >= monContrat.maxCoups) {
            printf("\n\nPERDU ! Plus de coups.\n");
            system("pause");
            break;
        }

        while (!toucheAppuyee()) {
            Sleep(100);
            printJeu(mat, &monContrat, curLig, curCol, selLig, selCol, 0);
        }

        Command cmd = recupererCommande();

        switch(cmd) {
            case CMD_UP:    if(curLig > 0) curLig--; break;
            case CMD_DOWN:  if(curLig < N-1) curLig++; break;
            case CMD_LEFT:  if(curCol > 0) curCol--; break;
            case CMD_RIGHT: if(curCol < N-1) curCol++; break;
            case CMD_QUIT:  running = 0; break;
            case CMD_SELECT:
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
                break;
            default: break;
        }
        printJeu(mat, &monContrat, curLig, curCol, selLig, selCol, 1);
    }
    free(mat);
}

int main(void) {
    srand((unsigned)time(NULL));
    initConsole();

    int choix = 0;
    while (1) {
        afficherMenu();
        if (scanf(" %d", &choix) != 1) {
            while(getchar() != '\n');
            continue;
        }

        switch (choix) {
            case 1: jouerPartie(); break;
            case 2: printf("\nIndisponible !\n"); Sleep(1000); break;
            case 3: return 0;
        }
    }
    return 0;
}