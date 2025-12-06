#ifndef UI_H
#define UI_H

#include "logique.h"

void Color(int text, int bg);
void initConsole();
void gotoligcol(int lig, int col);
void printJeu(char (*mat)[N], Contrat *c, int curLig, int curCol, int selLig, int selCol, int clear);
void afficherMenu();
int toucheAppuyee();
Command recupererCommande();

#endif