#ifndef UI_H
#define UI_H

#include "logique.h"

// Config initiale (UTF-8, Curseur caché)
void initConsole();

// Déplace le curseur à une position précise (Nouveau !)
void gotoligcol(int lig, int col);

// Affiche tout le jeu (Grille + HUD + Chrono)
// clear = 1 (Nettoyage total), clear = 0 (Rafraichissement simple)
void printJeu(char (*mat)[N], Contrat *c, int curLig, int curCol, int selLig, int selCol, int clear);

// Affiche le menu principal
void afficherMenu();

// Vérifie si une touche est appuyée (pour le chrono)
int toucheAppuyee();

// Attend et récupère une commande du joueur (UP, DOWN, QUIT, SELECT...)
Command recupererCommande();

#endif