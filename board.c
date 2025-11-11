#include "board.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

/**
 * Génère un caractère aléatoire (S, F, P, O, M)
 */
char random_item(void) {
    char items[] = {SUN, STRAWBERRY, APPLE, ONION, MANDARIN};
    return items[rand() % NUM_ITEMS];
}

/**
 * Vérifie si un groupe de 3+ items identiques existe à la position (row, col)
 */
int has_group_at(char board[ROWS][COLS], int row, int col) {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
        return 0;
    }
    
    char item = board[row][col];
    if (item == ' ' || item == '\0') {
        return 0;
    }
    
    int count_h = 1;  /* Horizontal */
    int count_v = 1;  /* Vertical */
    
    /* Vérification horizontale */
    int i = col - 1;
    while (i >= 0 && board[row][i] == item) {
        count_h++;
        i--;
    }
    i = col + 1;
    while (i < COLS && board[row][i] == item) {
        count_h++;
        i++;
    }
    
    /* Vérification verticale */
    i = row - 1;
    while (i >= 0 && board[i][col] == item) {
        count_v++;
        i--;
    }
    i = row + 1;
    while (i < ROWS && board[i][col] == item) {
        count_v++;
        i++;
    }
    
    return (count_h >= 3 || count_v >= 3);
}

/**
 * Vérifie si le plateau contient des groupes de 3+ items
 */
int has_any_groups(char board[ROWS][COLS]) {
    int row, col;
    for (row = 0; row < ROWS; row++) {
        for (col = 0; col < COLS; col++) {
            if (has_group_at(board, row, col)) {
                return 1;
            }
        }
    }
    return 0;
}

/**
 * Applique la gravité : fait tomber les items et remplit les vides
 */
void apply_gravity(char board[ROWS][COLS]) {
    int col, row, write_pos;
    
    /* Pour chaque colonne */
    for (col = 0; col < COLS; col++) {
        write_pos = ROWS - 1;
        
        /* Faire tomber les items existants */
        for (row = ROWS - 1; row >= 0; row--) {
            if (board[row][col] != ' ' && board[row][col] != '\0') {
                if (write_pos != row) {
                    board[write_pos][col] = board[row][col];
                    board[row][col] = ' ';
                }
                write_pos--;
            }
        }
        
        /* Remplir les vides en haut avec de nouveaux items aléatoires */
        while (write_pos >= 0) {
            board[write_pos][col] = random_item();
            write_pos--;
        }
    }
}

/**
 * Échange deux items sur le plateau
 */
int swap_items(char board[ROWS][COLS], int row1, int col1, int row2, int col2) {
    /* Vérifier les limites */
    if (row1 < 0 || row1 >= ROWS || col1 < 0 || col1 >= COLS ||
        row2 < 0 || row2 >= ROWS || col2 < 0 || col2 >= COLS) {
        return 0;
    }
    
    /* Vérifier que les positions sont adjacentes */
    int row_diff = (row1 > row2) ? (row1 - row2) : (row2 - row1);
    int col_diff = (col1 > col2) ? (col1 - col2) : (col2 - col1);
    if ((row_diff == 1 && col_diff == 0) || (row_diff == 0 && col_diff == 1)) {
        char temp = board[row1][col1];
        board[row1][col1] = board[row2][col2];
        board[row2][col2] = temp;
        return 1;
    }
    
    return 0;
}

/**
 * Initialise le plateau avec des caractères aléatoires
 * Garantit qu'aucun groupe de 3+ n'existe au départ
 */
void init_board(char board[ROWS][COLS]) {
    int row, col;
    int attempts = 0;
    const int max_attempts = 1000;
    
    /* Initialiser avec des caractères aléatoires */
    for (row = 0; row < ROWS; row++) {
        for (col = 0; col < COLS; col++) {
            board[row][col] = random_item();
        }
    }
    
    /* Vérifier et corriger les groupes */
    while (has_any_groups(board) && attempts < max_attempts) {
        for (row = 0; row < ROWS; row++) {
            for (col = 0; col < COLS; col++) {
                if (has_group_at(board, row, col)) {
                    /* Remplacer par un caractère différent */
                    char new_item;
                    do {
                        new_item = random_item();
                    } while (new_item == board[row][col]);
                    board[row][col] = new_item;
                }
            }
        }
        attempts++;
    }
}

