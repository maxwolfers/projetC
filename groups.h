#ifndef GROUPS_H
#define GROUPS_H

#include "config.h"

/* Structure pour stocker les items à supprimer */
typedef struct {
    int row;
    int col;
} Position;

/* Structure pour les résultats de détection */
typedef struct {
    int count;              /* Nombre d'items supprimés */
    int points;             /* Points gagnés */
    char item_type;         /* Type d'item supprimé */
} GroupResult;

/**
 * Détecte et supprime tous les groupes valides sur le plateau
 * @param board: plateau de jeu (IN/OUT)
 * @param contract_counts: compteurs de contrat à mettre à jour (IN/OUT)
 * @return structure GroupResult avec le nombre d'items supprimés et les points (OUT)
 */
GroupResult detect_and_remove_groups(char board[ROWS][COLS], 
                                     int contract_counts[]);

/**
 * Détecte un groupe de 4 items (horizontal ou vertical)
 * @param board: plateau de jeu (IN)
 * @param row: ligne de départ (IN)
 * @param col: colonne de départ (IN)
 * @param positions: tableau pour stocker les positions (OUT)
 * @return nombre d'items dans le groupe (OUT)
 */
int detect_group_4(char board[ROWS][COLS], int row, int col, Position positions[]);

/**
 * Détecte un groupe de 6 items (horizontal ou vertical)
 * @param board: plateau de jeu (IN)
 * @param row: ligne de départ (IN)
 * @param col: colonne de départ (IN)
 * @param positions: tableau pour stocker les positions (OUT)
 * @return nombre d'items dans le groupe (OUT)
 */
int detect_group_6(char board[ROWS][COLS], int row, int col, Position positions[]);

/**
 * Détecte un groupe en forme de H
 * @param board: plateau de jeu (IN)
 * @param row: ligne du centre (IN)
 * @param col: colonne du centre (IN)
 * @param positions: tableau pour stocker les positions (OUT)
 * @return nombre d'items dans le H (OUT)
 */
int detect_group_H(char board[ROWS][COLS], int row, int col, Position positions[]);

/**
 * Détecte un groupe en forme de rectangle
 * @param board: plateau de jeu (IN)
 * @param row: ligne de départ (IN)
 * @param col: colonne de départ (IN)
 * @param positions: tableau pour stocker les positions (OUT)
 * @return nombre d'items dans le rectangle (OUT)
 */
int detect_group_rectangle(char board[ROWS][COLS], int row, int col, Position positions[]);

/**
 * Détecte un groupe simple de 3+ items (horizontal ou vertical)
 * @param board: plateau de jeu (IN)
 * @param row: ligne de départ (IN)
 * @param col: colonne de départ (IN)
 * @param positions: tableau pour stocker les positions (OUT)
 * @return nombre d'items dans le groupe (OUT)
 */
int detect_group_3_plus(char board[ROWS][COLS], int row, int col, Position positions[]);

/**
 * Supprime tous les items identiques sur le plateau (pour groupe de 6)
 * @param board: plateau de jeu (IN/OUT)
 * @param item: type d'item à supprimer (IN)
 * @param contract_counts: compteurs de contrat (IN/OUT)
 * @return nombre d'items supprimés (OUT)
 */
int remove_all_items(char board[ROWS][COLS], char item, 
                     int contract_counts[]);

/**
 * Marque une position comme à supprimer
 * @param positions: tableau des positions (IN/OUT)
 * @param count: nombre actuel de positions (IN/OUT)
 * @param row: ligne (IN)
 * @param col: colonne (IN)
 */
void add_position(Position positions[], int *count, int row, int col);

/**
 * Vérifie si une position est déjà dans le tableau
 * @param positions: tableau des positions (IN)
 * @param count: nombre de positions (IN)
 * @param row: ligne (IN)
 * @param col: colonne (IN)
 * @return 1 si présente, 0 sinon (OUT)
 */
int position_exists(Position positions[], int count, int row, int col);

#endif /* GROUPS_H */

