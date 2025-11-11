#include "display.h"
#include "contract.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Efface l'écran
 */
void clear_screen(void) {
    printf("\033[2J");
    printf("\033[H");
}

/**
 * Déplace le curseur à la position (row, col)
 */
void move_cursor(int row, int col) {
    printf("\033[%d;%dH", row + 1, col + 1);
}

/**
 * Affiche un caractère avec sa couleur associée
 */
void print_colored_item(char item, int selected) {
    char display_char = selected ? (item + 32) : item; /* Minuscule si sélectionné */
    
    switch (item) {
        case SUN:
            printf("%s%c%s", COLOR_BRIGHT_YELLOW, display_char, COLOR_RESET);
            break;
        case STRAWBERRY:
            printf("%s%c%s", COLOR_BRIGHT_RED, display_char, COLOR_RESET);
            break;
        case APPLE:
            printf("%s%c%s", COLOR_GREEN, display_char, COLOR_RESET);
            break;
        case ONION:
            printf("%s%c%s", COLOR_WHITE, display_char, COLOR_RESET);
            break;
        case MANDARIN:
            printf("%s%c%s", COLOR_BRIGHT_CYAN, display_char, COLOR_RESET);
            break;
        default:
            printf("%c", display_char);
            break;
    }
}

/**
 * Affiche le plateau de jeu
 */
void display_board(char board[ROWS][COLS], int cursor_row, int cursor_col, 
                   int selected_row, int selected_col) {
    int row, col;
    
    /* Afficher une vue réduite centrée sur le curseur pour la lisibilité */
    int view_rows = 15;  /* Nombre de lignes à afficher */
    int view_cols = 30;   /* Nombre de colonnes à afficher */
    
    /* Calculer la zone à afficher (centrée sur le curseur) */
    int start_row = cursor_row - view_rows / 2;
    int start_col = cursor_col - view_cols / 2;
    
    if (start_row < 0) start_row = 0;
    if (start_col < 0) start_col = 0;
    if (start_row + view_rows > ROWS) start_row = ROWS - view_rows;
    if (start_col + view_cols > COLS) start_col = COLS - view_cols;
    
    printf("\n%s=== PLATEAU (vue partielle, centrée sur le curseur) ===%s\n", BOLD, COLOR_RESET);
    printf("Plateau complet: %d lignes x %d colonnes\n", ROWS, COLS);
    printf("Vue affichée: lignes %d-%d, colonnes %d-%d\n\n", 
           start_row, start_row + view_rows - 1, start_col, start_col + view_cols - 1);
    
    /* En-tête avec numéros de colonnes */
    printf("    ");
    for (col = start_col; col < start_col + view_cols && col < COLS; col++) {
        printf("%2d", col % 10);
    }
    printf("\n");
    
    /* Afficher les lignes */
    for (row = start_row; row < start_row + view_rows && row < ROWS; row++) {
        printf("%2d ", row);
        for (col = start_col; col < start_col + view_cols && col < COLS; col++) {
            int is_cursor = (row == cursor_row && col == cursor_col);
            int is_selected = (row == selected_row && col == selected_col);
            
            if (is_cursor) {
                printf("%s", REVERSE);
            }
            
            if (is_selected) {
                print_colored_item(board[row][col], 1);
            } else {
                print_colored_item(board[row][col], 0);
            }
            
            if (is_cursor) {
                printf("%s", COLOR_RESET);
            }
            
            printf(" ");
        }
        printf("\n");
    }
    
    /* Afficher la position du curseur */
    printf("\n%sCurseur: ligne %d, colonne %d%s", BOLD, cursor_row, cursor_col, COLOR_RESET);
    if (selected_row >= 0) {
        printf(" | %sItem sélectionné: ligne %d, colonne %d%s", BOLD, selected_row, selected_col, COLOR_RESET);
    }
    printf("\n");
}

/**
 * Affiche les informations du jeu
 */
void display_info(LevelContract contract, int contract_counts[], 
                  int moves_left, int time_left, int lives, int level) {
    char item_names[] = {SUN, STRAWBERRY, APPLE, ONION, MANDARIN};
    char *item_labels[] = {"Soleil", "Fraise", "Pomme", "Oignon", "Mandarine"};
    int i;
    
    printf("\n%s=== NIVEAU %d ===%s\n", BOLD, level, COLOR_RESET);
    printf("Contrat:\n");
    for (i = 0; i < NUM_ITEMS; i++) {
        if (contract.items[i] > 0) {
            printf("  %s: %d/%d ", item_labels[i], 
                   contract_counts[i], contract.items[i]);
            print_colored_item(item_names[i], 0);
            printf("\n");
        }
    }
    printf("Coups restants: %s%d%s\n", BOLD, moves_left, COLOR_RESET);
    printf("Temps restant: %s%d%s secondes\n", BOLD, time_left, COLOR_RESET);
    printf("Vies: %s%d%s\n", BOLD, lives, COLOR_RESET);
    printf("\n");
}

/**
 * Affiche le menu principal
 */
int display_menu(void) {
    int choice;
    clear_screen();
    printf("%s========================================%s\n", BOLD, COLOR_RESET);
    printf("%s      FARMHEROES - Jeu de Match-3      %s\n", BOLD, COLOR_RESET);
    printf("%s========================================%s\n", BOLD, COLOR_RESET);
    printf("\n");
    printf("1. Nouvelle partie\n");
    printf("2. Continuer la partie sauvegardée\n");
    printf("3. Quitter\n");
    printf("\nVotre choix: ");
    scanf("%d", &choice);
    return choice;
}

/**
 * Affiche un message de victoire
 */
void display_victory(int level) {
    clear_screen();
    printf("%s%s========================================%s\n", BOLD, COLOR_GREEN, COLOR_RESET);
    printf("%s%s      NIVEAU %d COMPLÉTÉ !      %s\n", BOLD, COLOR_GREEN, level, COLOR_RESET);
    printf("%s%s========================================%s\n", BOLD, COLOR_GREEN, COLOR_RESET);
    printf("\nFélicitations ! Vous avez réussi ce niveau.\n\n");
}

/**
 * Affiche un message de défaite
 */
void display_defeat(void) {
    clear_screen();
    printf("%s%s========================================%s\n", BOLD, COLOR_RED, COLOR_RESET);
    printf("%s%s         NIVEAU ÉCHOUÉ         %s\n", BOLD, COLOR_RED, COLOR_RESET);
    printf("%s%s========================================%s\n", BOLD, COLOR_RED, COLOR_RESET);
    printf("\nVous n'avez pas réussi à remplir le contrat à temps.\n\n");
}

/**
 * Affiche un message de fin de partie
 */
void display_game_over(void) {
    clear_screen();
    printf("%s%s========================================%s\n", BOLD, COLOR_RED, COLOR_RESET);
    printf("%s%s         GAME OVER         %s\n", BOLD, COLOR_RED, COLOR_RESET);
    printf("%s%s========================================%s\n", BOLD, COLOR_RED, COLOR_RESET);
    printf("\nVous avez épuisé toutes vos vies.\n\n");
}

