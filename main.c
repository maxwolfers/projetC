#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "logique.h"
#include "ui.h"

void jouerPartie(int charger) {
    char (*mat)[N] = malloc(sizeof *mat * N);
    if (!mat) return;

    Contrat monContrat;

    // 1. Initialisation (Chargement ou Niveau 1)
    if (charger) {
        if (!chargerPartie(mat, &monContrat)) {
            printf("\nAucune sauvegarde trouvee !\n");
            Sleep(1500);
            free(mat);
            return;
        }
    } else {
        initJeu(mat, &monContrat, 1); // On commence au niveau 1
        stabiliserPlateau(mat, &monContrat, 0);
    }

    system("cls");

    int curLig = 0, curCol = 0, selLig = -1, selCol = -1;
    int running = 1;

    printJeu(mat, &monContrat, curLig, curCol, selLig, selCol, 1);

    while (running) {

        // --- VERIFICATION VICTOIRE (CONTRAT REMPLI) ---
        if (monContrat.missions[0].mange >= monContrat.missions[0].aManger &&
            monContrat.missions[1].mange >= monContrat.missions[1].aManger &&
            monContrat.missions[2].mange >= monContrat.missions[2].aManger)
        {
            // Niveau terminé !
            system("cls");
            printf("\n\n========================================\n");
            printf("      NIVEAU %d TERMINE ! BRAVO ! \n", monContrat.niveau);
            printf("========================================\n\n");

            if (monContrat.niveau >= 3) {
                printf("FELICITATIONS ! VOUS AVEZ FINI LE JEU (3 Niveaux) !\n");
                system("pause");
                break; // Fin du jeu
            }

            printf("Voulez-vous passer au niveau %d ? (O/N) : ", monContrat.niveau + 1);

            // Petite boucle pour attendre O ou N
            while(1) {
                char c = _getch();
                if (c == 'o' || c == 'O') {
                    // Passage au niveau suivant
                    int scoreGardé = monContrat.score; // On garde le score ? (optionnel)
                    initJeu(mat, &monContrat, monContrat.niveau + 1);
                    monContrat.score = scoreGardé; // On remet le score cumulé

                    stabiliserPlateau(mat, &monContrat, 0);
                    printJeu(mat, &monContrat, curLig, curCol, selLig, selCol, 1);
                    break;
                }
                else if (c == 'n' || c == 'N') {
                    running = 0; // Quitter vers menu
                    break;
                }
            }
            if (!running) break; // Sortir de la boucle principale
        }

        // --- VERIFICATION DEFAITE ---
        if (monContrat.coups >= monContrat.maxCoups) {
            printf("\n\nPERDU ! Plus de coups.\n");
            system("pause");
            break;
        }

        // --- ATTENTE ET RAFRAICHISSEMENT CHRONO ---
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
                    printf(">> Partie Sauvegardee (Niveau %d) ! <<", monContrat.niveau);
                    Sleep(1000);
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

    int choix = 0;
    while (1) {
        afficherMenu();
        if (scanf(" %d", &choix) != 1) {
            while(getchar() != '\n');
            continue;
        }

        switch (choix) {
            case 1: jouerPartie(0); break; // Nouvelle partie
            case 2: jouerPartie(1); break; // Charger
            case 3: return 0;
        }
    }
    return 0;
}