#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "logique.h"
#include "ui.h"

void jouerPartie(int charger) {
    char (*mat)[N] = malloc(sizeof *mat * N);
    if (!mat) return;

    Contrat monContrat;

    if (charger) {
        // Tente de charger. Si échoue (fichier inexistant), on init une nouvelle partie
        if (!chargerPartie(mat, &monContrat)) {
            printf("Aucune sauvegarde trouvee ou erreur de lecture !\n");
            Sleep(1500); // Laisse le temps de lire
            initJeu(mat, &monContrat);
            stabiliserPlateau(mat, &monContrat, 0);
        }
    } else {
        // Nouvelle partie classique
        initJeu(mat, &monContrat);
        stabiliserPlateau(mat, &monContrat, 0);
    }

    system("cls");

    int curLig = 0, curCol = 0, selLig = -1, selCol = -1;
    int running = 1;

    printJeu(mat, &monContrat, curLig, curCol, selLig, selCol, 1);

    while (running) {
        // ... (gestion fin de partie inchangée) ...
        if (monContrat.coups >= monContrat.maxCoups) {
             // ... code perdu ...
             break;
        }

        while (!toucheAppuyee()) {
             // ... code attente ...
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
            
            // --- AJOUT GESTION SAUVEGARDE ---
            case CMD_SAVE:
                if (sauvegarderPartie(mat, &monContrat)) {
                    // Petit feedback visuel rapide
                    gotoligcol(N + 4, 0);
                    printf(">> PARTIE SAUVEGARDEE ! <<");
                    Sleep(1000);
                    // On efface le message
                    gotoligcol(N + 4, 0);
                    printf("                          ");
                }
                break;

            case CMD_SELECT:
                // ... (code sélection inchangé) ...
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
            case 1: jouerPartie(0); break; // 0 = Nouvelle partie
            case 2: jouerPartie(1); break; // 1 = Charger partie
            case 3: return 0;
        }
    }
    return 0;
}