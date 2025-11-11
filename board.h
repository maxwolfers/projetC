#ifndef BOARD_H
#define BOARD_H

#include "config.h"

/**
 * Initialise le plateau avec des caractères aléatoires
 * Garantit qu'aucun groupe de 3+ n'existe au départ
 * @param board: plateau à initialiser (OUT)
 */
void init_board(char board[ROWS][COLS]);

/**
 * Vérifie si un groupe de 3+ items identiques existe à la position (row, col)
 * @param board: plateau de jeu (IN)
 * @param row: ligne (IN)
 * @param col: colonne (IN)
 * @return 1 si un groupe existe, 0 sinon (OUT)
 */
int has_group_at(char board[ROWS][COLS], int row, int col);

/**
 * Vérifie si le plateau contient des groupes de 3+ items
 * @param board: plateau de jeu (IN)
 * @return 1 si des groupes existent, 0 sinon (OUT)
 */
int has_any_groups(char board[ROWS][COLS]);

/**
 * Applique la gravité : fait tomber les items et remplit les vides
 * @param board: plateau de jeu (IN/OUT)
 */
void apply_gravity(char board[ROWS][COLS]);

/**
 * Échange deux items sur le plateau
 * @param board: plateau de jeu (IN/OUT)
 * @param row1: ligne du premier item (IN)
 * @param col1: colonne du premier item (IN)
 * @param row2: ligne du second item (IN)
 * @param col2: colonne du second item (IN)
 * @return 1 si l'échange est valide, 0 sinon (OUT)
 */
int swap_items(char board[ROWS][COLS], int row1, int col1, int row2, int col2);

/**
 * Génère un caractère aléatoire (S, F, P, O, M)
 * @return caractère généré (OUT)
 */
char random_item(void);

#endif /* BOARD_H */

