#ifndef DISPLAY_H
#define DISPLAY_H

#include "config.h"
#include "contract.h"

/* Codes de couleur ANSI */
#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"
#define COLOR_WHITE "\033[37m"
#define COLOR_BRIGHT_RED "\033[91m"
#define COLOR_BRIGHT_YELLOW "\033[93m"
#define COLOR_BRIGHT_CYAN "\033[96m"

/* Couleurs de fond */
#define BG_BLACK "\033[40m"
#define BG_WHITE "\033[47m"
#define BG_RED "\033[41m"

/* Effets */
#define BOLD "\033[1m"
#define REVERSE "\033[7m"

/**
 * Efface l'écran
 */
void clear_screen(void);

/**
 * Déplace le curseur à la position (row, col)
 * @param row: ligne (IN)
 * @param col: colonne (IN)
 */
void move_cursor(int row, int col);

/**
 * Affiche un caractère avec sa couleur associée
 * @param item: caractère à afficher (IN)
 * @param selected: 1 si sélectionné (minuscule), 0 sinon (IN)
 */
void print_colored_item(char item, int selected);

/**
 * Affiche le plateau de jeu
 * @param board: plateau de jeu (IN)
 * @param cursor_row: ligne du curseur (IN)
 * @param cursor_col: colonne du curseur (IN)
 * @param selected_row: ligne de l'item sélectionné (-1 si aucun) (IN)
 * @param selected_col: colonne de l'item sélectionné (-1 si aucun) (IN)
 */
void display_board(char board[ROWS][COLS], int cursor_row, int cursor_col, 
                   int selected_row, int selected_col);

/**
 * Affiche les informations du jeu (contrat, coups, temps, vies)
 * @param contract: structure LevelContract avec les objectifs (IN)
 * @param contract_counts: tableau des compteurs actuels (IN)
 * @param moves_left: coups restants (IN)
 * @param time_left: temps restant en secondes (IN)
 * @param lives: vies restantes (IN)
 * @param level: niveau actuel (IN)
 */
void display_info(LevelContract contract, int contract_counts[], 
                  int moves_left, int time_left, int lives, int level);

/**
 * Affiche le menu principal
 * @return choix de l'utilisateur (OUT)
 */
int display_menu(void);

/**
 * Affiche un message de victoire
 * @param level: niveau complété (IN)
 */
void display_victory(int level);

/**
 * Affiche un message de défaite
 */
void display_defeat(void);

/**
 * Affiche un message de fin de partie
 */
void display_game_over(void);

#endif /* DISPLAY_H */

