#ifndef INPUT_H
#define INPUT_H

#include "config.h"

/* Codes de touches */
#define KEY_UP 'z'
#define KEY_DOWN 's'
#define KEY_LEFT 'q'
#define KEY_RIGHT 'd'
#define KEY_SELECT ' '  /* Barre d'espace */
#define KEY_QUIT 'x'
#define KEY_ENTER '\n'

/* Codes pour les flèches (séquence ANSI) */
#define ARROW_UP 65
#define ARROW_DOWN 66
#define ARROW_RIGHT 67
#define ARROW_LEFT 68

/**
 * Configure le terminal en mode non-canonique pour la saisie
 */
void setup_input(void);

/**
 * Restaure le terminal en mode canonique
 */
void restore_input(void);

/**
 * Lit une touche pressée (non-bloquant)
 * @return code de la touche, 0 si aucune touche (OUT)
 */
int get_key(void);

/**
 * Attend qu'une touche soit pressée (bloquant)
 * @return code de la touche (OUT)
 */
int wait_key(void);

/**
 * Gère le déplacement du curseur
 * @param cursor_row: ligne actuelle du curseur (IN/OUT)
 * @param cursor_col: colonne actuelle du curseur (IN/OUT)
 * @param key: touche pressée (IN)
 */
void handle_cursor_move(int *cursor_row, int *cursor_col, int key);

#endif /* INPUT_H */

