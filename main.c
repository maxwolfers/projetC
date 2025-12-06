#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "logique.h"
#include "ui.h"
#include "audio.h"

void jouerPartie(int charger, int *musiqueActive) {
    char (*mat)[N] = malloc(sizeof *mat * N);
    if (!mat) return;

    Contrat monContrat;

    if (charger) {
        if (!chargerPartie(mat, &monContrat)) {
            printf("\nAucune sauvegarde trouvee !\n");
            Sleep(1500);
            free(mat);
            return;
        }
    } else {
        initJeu(mat, &monContrat, 1);
        stabiliserPlateau(mat, &monContrat, 0);
    }

    system("cls");

    int curLig = 0, curCol = 0, selLig = -1, selCol = -1;
    int running = 1;

    printJeu(mat, &monContrat, curLig, curCol, selLig, selCol, 1);

    while (running) {
        if (monContrat.missions[0].mange >= monContrat.missions[0].aManger &&
            monContrat.missions[1].mange >= monContrat.missions[1].aManger &&
            monContrat.missions[2].mange >= monContrat.missions[2].aManger)
        {
            system("cls");
            printf("\n\n========================================\n");
            printf("      NIVEAU %d TERMINE ! BRAVO ! \n", monContrat.niveau);
            printf("========================================\n\n");

            if (monContrat.niveau >= 3) {
                printf("FELICITATIONS ! VOUS AVEZ FINI LE JEU !\n");
                system("pause");
                break;
            }

            printf("Appuyez sur une touche pour le niveau %d...", monContrat.niveau + 1);
            getch();

            int scoreActuel = monContrat.score;
            int viesActuelles = monContrat.vies;

            initJeu(mat, &monContrat, monContrat.niveau + 1);

            monContrat.score = scoreActuel;
            monContrat.vies = viesActuelles;

            stabiliserPlateau(mat, &monContrat, 0);
            printJeu(mat, &monContrat, curLig, curCol, selLig, selCol, 1);
        }

        if (monContrat.coups >= monContrat.maxCoups) {
            monContrat.vies--;

            system("cls");
            Color(12, 0);
            printf("\n========================================\n");
            printf("      PLUS DE COUPS ! VIE PERDUE \n");
            printf("========================================\n\n");
            Color(15, 0);

            if (monContrat.vies <= 0) {
                printf("GAME OVER ! Vous n'avez plus de vies.\n");
                system("pause");
                break;
            } else {
                printf("Il vous reste %d vies.\n", monContrat.vies);
                printf("Voulez-vous recommencer ce niveau ? (O/N) : ");

                while(1) {
                    char c = getch();
                    if (c == 'o' || c == 'O') {
                        int scoreActuel = monContrat.score;
                        int viesRestantes = monContrat.vies;
                        int niveauActuel = monContrat.niveau;

                        initJeu(mat, &monContrat, niveauActuel);

                        monContrat.vies = viesRestantes;
                        monContrat.score = scoreActuel;

                        stabiliserPlateau(mat, &monContrat, 0);
                        printJeu(mat, &monContrat, curLig, curCol, selLig, selCol, 1);
                        break;
                    }
                    else if (c == 'n' || c == 'N') {
                        running = 0;
                        break;
                    }
                }
                if (!running) break;
            }
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

            case CMD_SAVE:
                if (sauvegarderPartie(mat, &monContrat)) {
                    gotoligcol(N + 4, 0);
                    printf(">> Partie Sauvegardee ! <<");
                    Sleep(1000);
                }
                break;

            case CMD_MUTE:
                if (*musiqueActive) {
                    arreterMusique();
                    *musiqueActive = 0;
                } else {
                    lancerMusique("musique.wav");
                    *musiqueActive = 1;
                }
                break;

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

    int musiqueActive = 0;
    // lancerMusique("musique.wav"); musiqueActive = 1; // Decommente pour lancer au debut

    int running = 1;
    while (running) {
        afficherMenu(musiqueActive);

        int key = _getch();

        if (key == '1') {
            jouerPartie(0, &musiqueActive);
        }
        else if (key == '2') {
            jouerPartie(1, &musiqueActive);
        }
        else if (key == '3') {
            running = 0;
        }
        else if (key == 'm' || key == 'M') {
            // Bascule Musique dans le menu
            if (musiqueActive) {
                arreterMusique();
                musiqueActive = 0;
            } else {
                lancerMusique("musique.wav");
                musiqueActive = 1;
            }
        }
    }
    return 0;
}