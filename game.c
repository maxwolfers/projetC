#include "game.h"
#include "board.h"
#include "groups.h"
#include "display.h"
#include "input.h"
#include "contract.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/**
 * Initialise un nouvel état de jeu
 */
void init_game_state(GameState *state, int level) {
    int i;
    
    /* Initialiser le plateau */
    srand((unsigned int)time(NULL));
    init_board(state->board);
    
    /* Générer le contrat */
    state->contract = generate_contract(level);
    state->level = level;
    
    /* Initialiser les compteurs */
    init_contract_counts(state->contract_counts);
    
    /* Initialiser les autres variables */
    state->moves_left = state->contract.max_moves;
    state->time_left = state->contract.time_limit;
    state->lives = INITIAL_LIVES;
    state->cursor_row = ROWS / 2;
    state->cursor_col = COLS / 2;
    state->selected_row = -1;
    state->selected_col = -1;
    state->start_time = time(NULL);
    
    /* Initialiser les données de sauvegarde */
    strcpy(state->save_data.player_name, "Joueur");
    state->save_data.level = level;
    state->save_data.lives = INITIAL_LIVES;
    for (i = 0; i < MAX_LEVELS; i++) {
        state->save_data.completed_levels[i] = 0;
    }
}

/**
 * Met à jour le temps restant
 */
void update_time(GameState *state) {
    time_t current_time = time(NULL);
    int elapsed = (int)(current_time - state->start_time);
    state->time_left = state->contract.time_limit - elapsed;
    if (state->time_left < 0) {
        state->time_left = 0;
    }
}

/**
 * Vérifie si le temps est écoulé
 */
int is_time_up(GameState *state) {
    return (state->time_left <= 0);
}

/**
 * Stabilise le plateau (applique gravité et supprime groupes jusqu'à stabilisation)
 */
void stabilize_board(GameState *state) {
    int stabilized = 0;
    int iterations = 0;
    const int max_iterations = 100;
    
    while (!stabilized && iterations < max_iterations) {
        stabilized = 1;
        iterations++;
        
        /* Détecter et supprimer les groupes */
        GroupResult result = detect_and_remove_groups(state->board, 
                                                      state->contract_counts);
        
        if (result.count > 0) {
            stabilized = 0;
            /* La gravité est déjà appliquée dans detect_and_remove_groups */
        }
    }
}

/**
 * Traite un coup du joueur
 */
int process_move(GameState *state, int key) {
    int moved = 0;
    
    /* Gestion de la sélection/désélection */
    if (key == KEY_SELECT || key == ' ') {
        if (state->selected_row == -1) {
            /* Sélectionner l'item */
            state->selected_row = state->cursor_row;
            state->selected_col = state->cursor_col;
        } else if (state->selected_row == state->cursor_row && 
                   state->selected_col == state->cursor_col) {
            /* Désélectionner si on appuie sur le même item */
            state->selected_row = -1;
            state->selected_col = -1;
        } else {
            /* Échanger avec l'item sélectionné */
            int new_row = state->cursor_row;
            int new_col = state->cursor_col;
            
            /* Vérifier si adjacent */
            int row_diff = (state->selected_row > new_row) ? 
                          (state->selected_row - new_row) : 
                          (new_row - state->selected_row);
            int col_diff = (state->selected_col > new_col) ? 
                          (state->selected_col - new_col) : 
                          (new_col - state->selected_col);
            
            if ((row_diff == 1 && col_diff == 0) || 
                (row_diff == 0 && col_diff == 1)) {
                /* Échanger */
                if (swap_items(state->board, state->selected_row, state->selected_col,
                              new_row, new_col)) {
                    /* Vérifier si un groupe est formé */
                    GroupResult result = detect_and_remove_groups(state->board,
                                                                 state->contract_counts);
                    
                    if (result.count > 0) {
                        /* Un groupe a été formé, stabiliser le plateau */
                        stabilize_board(state);
                        state->moves_left--;
                        moved = 1;
                    } else {
                        /* Aucun groupe formé, annuler l'échange */
                        swap_items(state->board, state->selected_row, state->selected_col,
                                  new_row, new_col);
                    }
                }
            }
            
            /* Désélectionner */
            state->selected_row = -1;
            state->selected_col = -1;
        }
    } else {
        /* Déplacer le curseur */
        handle_cursor_move(&state->cursor_row, &state->cursor_col, key);
    }
    
    return moved;
}

/**
 * Boucle principale du jeu
 */
int game_loop(GameState *state) {
    int key;
    int game_running = 1;
    int result = 0; /* 1 = victoire, 0 = défaite, -1 = quitte */
    
    setup_input();
    
    while (game_running) {
        /* Mettre à jour le temps */
        update_time(state);
        
        /* Vérifier les conditions de fin */
        if (is_time_up(state)) {
            result = 0; /* Défaite */
            game_running = 0;
            break;
        }
        
        if (state->moves_left <= 0) {
            /* Vérifier si le contrat est rempli */
            if (is_contract_complete(state->contract, state->contract_counts)) {
                result = 1; /* Victoire */
            } else {
                result = 0; /* Défaite */
            }
            game_running = 0;
            break;
        }
        
        if (is_contract_complete(state->contract, state->contract_counts)) {
            result = 1; /* Victoire */
            game_running = 0;
            break;
        }
        
        /* Afficher le plateau */
        clear_screen();
        display_board(state->board, state->cursor_row, state->cursor_col,
                     state->selected_row, state->selected_col);
        
        /* Afficher les informations */
        display_info(state->contract, state->contract_counts,
                    state->moves_left, state->time_left, state->lives, state->level);
        
        printf("\n%s=== CONTRÔLES ===%s\n", BOLD, COLOR_RESET);
        printf("z/q/s/d ou flèches : Déplacer le curseur\n");
        printf("ESPACE : Sélectionner/désélectionner ou échanger\n");
        printf("x : Quitter le jeu\n");
        printf("\n%sAppuyez sur une touche pour jouer...%s\n", BOLD, COLOR_RESET);
        
        /* Lire l'input */
        key = get_key();
        
        if (key == KEY_QUIT || key == 'x' || key == 'X') {
            result = -1; /* Quitte */
            game_running = 0;
            break;
        }
        
        if (key != 0) {
            process_move(state, key);
        }
        
        /* Petit délai pour éviter de surcharger le CPU */
        usleep(50000); /* 50ms */
    }
    
    restore_input();
    return result;
}

