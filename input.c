#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

static struct termios old_termios;

/**
 * Configure le terminal en mode non-canonique pour la saisie
 */
void setup_input(void) {
    struct termios new_termios;
    
    /* Vérifier que stdin est un terminal */
    if (!isatty(STDIN_FILENO)) {
        return;
    }
    
    tcgetattr(STDIN_FILENO, &old_termios);
    new_termios = old_termios;
    
    /* Mode non-canonique, pas d'écho */
    new_termios.c_lflag &= ~(ICANON | ECHO);
    new_termios.c_cc[VMIN] = 0;
    new_termios.c_cc[VTIME] = 0;
    
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
    
    /* Mode non-bloquant */
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
    
    /* Cacher le curseur */
    printf("\033[?25l");
    fflush(stdout);
}

/**
 * Restaure le terminal en mode canonique
 */
void restore_input(void) {
    /* Vérifier que stdin est un terminal */
    if (!isatty(STDIN_FILENO)) {
        return;
    }
    
    /* Restaurer le curseur */
    printf("\033[?25h");
    fflush(stdout);
    
    tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
    fcntl(STDIN_FILENO, F_SETFL, 0);
}

/**
 * Lit une touche pressée (non-bloquant)
 */
int get_key(void) {
    char c;
    if (read(STDIN_FILENO, &c, 1) == 1) {
        /* Gestion des flèches (séquence ESC [ A/B/C/D) */
        if (c == '\033') {
            if (read(STDIN_FILENO, &c, 1) == 1 && c == '[') {
                if (read(STDIN_FILENO, &c, 1) == 1) {
                    switch (c) {
                        case ARROW_UP:
                            return KEY_UP;
                        case ARROW_DOWN:
                            return KEY_DOWN;
                        case ARROW_LEFT:
                            return KEY_LEFT;
                        case ARROW_RIGHT:
                            return KEY_RIGHT;
                    }
                }
            }
            return 0; /* ESC seul */
        }
        return c;
    }
    return 0;
}

/**
 * Attend qu'une touche soit pressée (bloquant)
 */
int wait_key(void) {
    int key;
    while ((key = get_key()) == 0) {
        usleep(10000); /* 10ms */
    }
    return key;
}

/**
 * Gère le déplacement du curseur
 */
void handle_cursor_move(int *cursor_row, int *cursor_col, int key) {
    switch (key) {
        case KEY_UP:
        case 'Z':
            if (*cursor_row > 0) {
                (*cursor_row)--;
            }
            break;
        case KEY_DOWN:
        case 'S':
            if (*cursor_row < ROWS - 1) {
                (*cursor_row)++;
            }
            break;
        case KEY_LEFT:
        case 'Q':
            if (*cursor_col > 0) {
                (*cursor_col)--;
            }
            break;
        case KEY_RIGHT:
        case 'D':
            if (*cursor_col < COLS - 1) {
                (*cursor_col)++;
            }
            break;
    }
}

