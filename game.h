#ifndef GAME_H
#define GAME_H

#include "config.h"
#include "contract.h"
#include "save.h"
#include <time.h>

/* Structure principale du jeu */
typedef struct {
    char board[ROWS][COLS];
    LevelContract contract;
    int contract_counts[NUM_ITEMS];
    int moves_left;
    int time_left;
    int lives;
    int level;
    int cursor_row;
    int cursor_col;
    int selected_row;
    int selected_col;
    time_t start_time;
    SaveData save_data;
} GameState;

/**
 * Initialise un nouvel état de jeu
 * @param state: état à initialiser (OUT)
 * @param level: niveau à charger (IN)
 */
void init_game_state(GameState *state, int level);

/**
 * Boucle principale du jeu
 * @param state: état du jeu (IN/OUT)
 * @return 1 si victoire, 0 si défaite, -1 si quitte (OUT)
 */
int game_loop(GameState *state);

/**
 * Met à jour le temps restant
 * @param state: état du jeu (IN/OUT)
 */
void update_time(GameState *state);

/**
 * Vérifie si le temps est écoulé
 * @param state: état du jeu (IN)
 * @return 1 si temps écoulé, 0 sinon (OUT)
 */
int is_time_up(GameState *state);

/**
 * Traite un coup du joueur
 * @param state: état du jeu (IN/OUT)
 * @param key: touche pressée (IN)
 * @return 1 si un coup a été joué, 0 sinon (OUT)
 */
int process_move(GameState *state, int key);

/**
 * Stabilise le plateau (applique gravité et supprime groupes jusqu'à stabilisation)
 * @param state: état du jeu (IN/OUT)
 */
void stabilize_board(GameState *state);

#endif /* GAME_H */

