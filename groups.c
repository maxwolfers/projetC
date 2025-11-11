#include "groups.h"
#include "board.h"
#include <stdlib.h>
#include <string.h>

/**
 * Vérifie si une position est déjà dans le tableau
 */
int position_exists(Position positions[], int count, int row, int col) {
    int i;
    for (i = 0; i < count; i++) {
        if (positions[i].row == row && positions[i].col == col) {
            return 1;
        }
    }
    return 0;
}

/**
 * Marque une position comme à supprimer
 */
void add_position(Position positions[], int *count, int row, int col) {
    if (!position_exists(positions, *count, row, col)) {
        positions[*count].row = row;
        positions[*count].col = col;
        (*count)++;
    }
}

/**
 * Détecte un groupe simple de 3+ items (horizontal ou vertical)
 */
int detect_group_3_plus(char board[ROWS][COLS], int row, int col, Position positions[]) {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
        return 0;
    }
    
    char item = board[row][col];
    if (item == ' ' || item == '\0') {
        return 0;
    }
    
    int count = 0;
    int i;
    
    /* Vérification horizontale */
    int start_col = col;
    while (start_col > 0 && board[row][start_col - 1] == item) {
        start_col--;
    }
    int end_col = col;
    while (end_col < COLS - 1 && board[row][end_col + 1] == item) {
        end_col++;
    }
    
    if (end_col - start_col + 1 >= 3) {
        for (i = start_col; i <= end_col; i++) {
            add_position(positions, &count, row, i);
        }
    }
    
    /* Vérification verticale */
    int start_row = row;
    while (start_row > 0 && board[start_row - 1][col] == item) {
        start_row--;
    }
    int end_row = row;
    while (end_row < ROWS - 1 && board[end_row + 1][col] == item) {
        end_row++;
    }
    
    if (end_row - start_row + 1 >= 3) {
        for (i = start_row; i <= end_row; i++) {
            add_position(positions, &count, i, col);
        }
    }
    
    return count;
}

/**
 * Détecte un groupe de 4 items (horizontal ou vertical)
 */
int detect_group_4(char board[ROWS][COLS], int row, int col, Position positions[]) {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
        return 0;
    }
    
    char item = board[row][col];
    if (item == ' ' || item == '\0') {
        return 0;
    }
    
    int count = 0;
    int i;
    
    /* Vérification horizontale */
    if (col >= 3) {
        int match = 1;
        for (i = 0; i < 4; i++) {
            if (board[row][col - i] != item) {
                match = 0;
                break;
            }
        }
        if (match) {
            for (i = 0; i < 4; i++) {
                add_position(positions, &count, row, col - i);
            }
            return count;
        }
    }
    if (col <= COLS - 4) {
        int match = 1;
        for (i = 0; i < 4; i++) {
            if (board[row][col + i] != item) {
                match = 0;
                break;
            }
        }
        if (match) {
            for (i = 0; i < 4; i++) {
                add_position(positions, &count, row, col + i);
            }
            return count;
        }
    }
    
    /* Vérification verticale */
    if (row >= 3) {
        int match = 1;
        for (i = 0; i < 4; i++) {
            if (board[row - i][col] != item) {
                match = 0;
                break;
            }
        }
        if (match) {
            for (i = 0; i < 4; i++) {
                add_position(positions, &count, row - i, col);
            }
            return count;
        }
    }
    if (row <= ROWS - 4) {
        int match = 1;
        for (i = 0; i < 4; i++) {
            if (board[row + i][col] != item) {
                match = 0;
                break;
            }
        }
        if (match) {
            for (i = 0; i < 4; i++) {
                add_position(positions, &count, row + i, col);
            }
            return count;
        }
    }
    
    return 0;
}

/**
 * Détecte un groupe de 6 items (horizontal ou vertical)
 */
int detect_group_6(char board[ROWS][COLS], int row, int col, Position positions[]) {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
        return 0;
    }
    
    char item = board[row][col];
    if (item == ' ' || item == '\0') {
        return 0;
    }
    
    int count = 0;
    int i;
    
    /* Vérification horizontale */
    if (col >= 5) {
        int match = 1;
        for (i = 0; i < 6; i++) {
            if (board[row][col - i] != item) {
                match = 0;
                break;
            }
        }
        if (match) {
            for (i = 0; i < 6; i++) {
                add_position(positions, &count, row, col - i);
            }
            return count;
        }
    }
    if (col <= COLS - 6) {
        int match = 1;
        for (i = 0; i < 6; i++) {
            if (board[row][col + i] != item) {
                match = 0;
                break;
            }
        }
        if (match) {
            for (i = 0; i < 6; i++) {
                add_position(positions, &count, row, col + i);
            }
            return count;
        }
    }
    
    /* Vérification verticale */
    if (row >= 5) {
        int match = 1;
        for (i = 0; i < 6; i++) {
            if (board[row - i][col] != item) {
                match = 0;
                break;
            }
        }
        if (match) {
            for (i = 0; i < 6; i++) {
                add_position(positions, &count, row - i, col);
            }
            return count;
        }
    }
    if (row <= ROWS - 6) {
        int match = 1;
        for (i = 0; i < 6; i++) {
            if (board[row + i][col] != item) {
                match = 0;
                break;
            }
        }
        if (match) {
            for (i = 0; i < 6; i++) {
                add_position(positions, &count, row + i, col);
            }
            return count;
        }
    }
    
    return 0;
}

/**
 * Détecte un groupe en forme de H
 * Un H est formé de 3 lignes horizontales et 1 ligne verticale au centre
 */
int detect_group_H(char board[ROWS][COLS], int row, int col, Position positions[]) {
    if (row < 1 || row >= ROWS - 1 || col < 1 || col >= COLS - 1) {
        return 0;
    }
    
    char item = board[row][col];
    if (item == ' ' || item == '\0') {
        return 0;
    }
    
    /* Vérifier les différentes orientations de H */
    int count = 0;
    int i;
    
    /* H horizontal (barre horizontale en haut, bas, et verticale au milieu) */
    if (col >= 1 && col < COLS - 1) {
        /* H avec barre horizontale en haut et bas */
        if (row >= 1 && row < ROWS - 1) {
            /* Barre du haut */
            if (board[row - 1][col - 1] == item && 
                board[row - 1][col] == item && 
                board[row - 1][col + 1] == item) {
                /* Barre du bas */
                if (board[row + 1][col - 1] == item && 
                    board[row + 1][col] == item && 
                    board[row + 1][col + 1] == item) {
                    /* Barre verticale */
                    if (board[row][col] == item) {
                        /* Ajouter toutes les positions */
                        for (i = -1; i <= 1; i++) {
                            add_position(positions, &count, row - 1, col + i);
                            add_position(positions, &count, row + 1, col + i);
                        }
                        add_position(positions, &count, row, col);
                        return count;
                    }
                }
            }
        }
    }
    
    /* H vertical (barre verticale à gauche, droite, et horizontale au milieu) */
    if (row >= 1 && row < ROWS - 1) {
        if (col >= 1 && col < COLS - 1) {
            /* Barre de gauche */
            if (board[row - 1][col - 1] == item && 
                board[row][col - 1] == item && 
                board[row + 1][col - 1] == item) {
                /* Barre de droite */
                if (board[row - 1][col + 1] == item && 
                    board[row][col + 1] == item && 
                    board[row + 1][col + 1] == item) {
                    /* Barre horizontale */
                    if (board[row][col] == item) {
                        /* Ajouter toutes les positions */
                        for (i = -1; i <= 1; i++) {
                            add_position(positions, &count, row + i, col - 1);
                            add_position(positions, &count, row + i, col + 1);
                        }
                        add_position(positions, &count, row, col);
                        return count;
                    }
                }
            }
        }
    }
    
    return 0;
}

/**
 * Détecte un groupe en forme de rectangle
 */
int detect_group_rectangle(char board[ROWS][COLS], int row, int col, Position positions[]) {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
        return 0;
    }
    
    char item = board[row][col];
    if (item == ' ' || item == '\0') {
        return 0;
    }
    
    int max_size = 0;
    int best_width = 0, best_height = 0;
    int best_start_row = row, best_start_col = col;
    int width, height;
    int i, j;
    int valid;
    
    /* Essayer différentes tailles de rectangles à partir de (row, col) */
    for (height = 2; height <= ROWS - row && height <= 10; height++) {
        for (width = 2; width <= COLS - col && width <= 10; width++) {
            valid = 1;
            
            /* Vérifier si tous les items du rectangle sont identiques */
            for (i = 0; i < height && valid; i++) {
                for (j = 0; j < width && valid; j++) {
                    if (board[row + i][col + j] != item) {
                        valid = 0;
                    }
                }
            }
            
            if (valid && (width * height > max_size)) {
                max_size = width * height;
                best_width = width;
                best_height = height;
                best_start_row = row;
                best_start_col = col;
            }
        }
    }
    
    /* Si un rectangle valide a été trouvé (minimum 2x2 = 4 items) */
    if (max_size >= 4) {
        int count = 0;
        for (i = 0; i < best_height; i++) {
            for (j = 0; j < best_width; j++) {
                add_position(positions, &count, best_start_row + i, best_start_col + j);
            }
        }
        return count;
    }
    
    return 0;
}

/**
 * Supprime tous les items identiques sur le plateau (pour groupe de 6)
 */
int remove_all_items(char board[ROWS][COLS], char item, 
                     int contract_counts[]) {
    int count = 0;
    int row, col;
    
    for (row = 0; row < ROWS; row++) {
        for (col = 0; col < COLS; col++) {
            if (board[row][col] == item) {
                board[row][col] = ' ';
                count++;
            }
        }
    }
    
    /* Mettre à jour les compteurs de contrat */
    if (count > 0) {
        char items[] = {SUN, STRAWBERRY, APPLE, ONION, MANDARIN};
        int item_idx = -1;
        int i;
        
        for (i = 0; i < NUM_ITEMS; i++) {
            if (items[i] == item) {
                item_idx = i;
                break;
            }
        }
        
        if (item_idx >= 0) {
            contract_counts[item_idx] += count;
        }
    }
    
    return count;
}

/**
 * Détecte et supprime tous les groupes valides sur le plateau
 */
GroupResult detect_and_remove_groups(char board[ROWS][COLS], 
                                     int contract_counts[]) {
    GroupResult result = {0, 0, ' '};
    int total_removed = 0;
    int total_points = 0;
    char item_type = ' ';
    int row, col;
    int found_any = 1;
    int iteration = 0;
    const int max_iterations = 50; /* Éviter les boucles infinies */
    
    /* Répéter jusqu'à ce qu'aucun groupe ne soit trouvé */
    while (found_any && iteration < max_iterations) {
        found_any = 0;
        iteration++;
        
        /* Parcourir le plateau */
        for (row = 0; row < ROWS; row++) {
            for (col = 0; col < COLS; col++) {
                if (board[row][col] == ' ' || board[row][col] == '\0') {
                    continue;
                }
                
                int pos_count = 0;
                Position temp_positions[ROWS * COLS];
                char current_item = board[row][col];
                int points = 0;
                int removed = 0;
                int i; /* Variable de boucle */
                
                /* Vérifier d'abord les figures spéciales (priorité) */
                
                /* 1. Groupe de 6 (supprime tous les items identiques) */
                pos_count = detect_group_6(board, row, col, temp_positions);
                if (pos_count >= 6) {
                    removed = remove_all_items(board, current_item, contract_counts);
                    points = removed; /* 1 point par item */
                    found_any = 1;
                    item_type = current_item;
                    total_removed += removed;
                    total_points += points;
                    break; /* Sortir de la boucle pour réappliquer la gravité */
                }
                
                /* 2. Rectangle */
                pos_count = detect_group_rectangle(board, row, col, temp_positions);
                if (pos_count >= 4) {
                    /* Calculer L x l */
                    int min_row = temp_positions[0].row, max_row = temp_positions[0].row;
                    int min_col = temp_positions[0].col, max_col = temp_positions[0].col;
                    
                    for (i = 1; i < pos_count; i++) {
                        if (temp_positions[i].row < min_row) min_row = temp_positions[i].row;
                        if (temp_positions[i].row > max_row) max_row = temp_positions[i].row;
                        if (temp_positions[i].col < min_col) min_col = temp_positions[i].col;
                        if (temp_positions[i].col > max_col) max_col = temp_positions[i].col;
                    }
                    
                    int length = max_row - min_row + 1;
                    int width = max_col - min_col + 1;
                    points = 2 * (length * width);
                    
                    /* Supprimer les items */
                    for (i = 0; i < pos_count; i++) {
                        if (board[temp_positions[i].row][temp_positions[i].col] == current_item) {
                            board[temp_positions[i].row][temp_positions[i].col] = ' ';
                            removed++;
                        }
                    }
                    
                    /* Mettre à jour le contrat */
                    char items[] = {SUN, STRAWBERRY, APPLE, ONION, MANDARIN};
                    int item_idx = -1;
                    for (i = 0; i < NUM_ITEMS; i++) {
                        if (items[i] == current_item) {
                            item_idx = i;
                            break;
                        }
                    }
                    if (item_idx >= 0) {
                        contract_counts[item_idx] += removed;
                    }
                    
                    found_any = 1;
                    item_type = current_item;
                    total_removed += removed;
                    total_points += points;
                    break;
                }
                
                /* 3. H */
                pos_count = detect_group_H(board, row, col, temp_positions);
                if (pos_count >= 7) { /* Un H a au moins 7 items */
                    points = 2 * pos_count;
                    
                    /* Supprimer les items */
                    for (i = 0; i < pos_count; i++) {
                        if (board[temp_positions[i].row][temp_positions[i].col] == current_item) {
                            board[temp_positions[i].row][temp_positions[i].col] = ' ';
                            removed++;
                        }
                    }
                    
                    /* Mettre à jour le contrat */
                    char items[] = {SUN, STRAWBERRY, APPLE, ONION, MANDARIN};
                    int item_idx = -1;
                    for (i = 0; i < NUM_ITEMS; i++) {
                        if (items[i] == current_item) {
                            item_idx = i;
                            break;
                        }
                    }
                    if (item_idx >= 0) {
                        contract_counts[item_idx] += removed;
                    }
                    
                    found_any = 1;
                    item_type = current_item;
                    total_removed += removed;
                    total_points += points;
                    break;
                }
                
                /* 4. Groupe de 4 */
                pos_count = detect_group_4(board, row, col, temp_positions);
                if (pos_count == 4) {
                    points = 4;
                    
                    /* Supprimer les items */
                    for (i = 0; i < pos_count; i++) {
                        if (board[temp_positions[i].row][temp_positions[i].col] == current_item) {
                            board[temp_positions[i].row][temp_positions[i].col] = ' ';
                            removed++;
                        }
                    }
                    
                    /* Mettre à jour le contrat */
                    char items[] = {SUN, STRAWBERRY, APPLE, ONION, MANDARIN};
                    int item_idx = -1;
                    for (i = 0; i < NUM_ITEMS; i++) {
                        if (items[i] == current_item) {
                            item_idx = i;
                            break;
                        }
                    }
                    if (item_idx >= 0) {
                        contract_counts[item_idx] += removed;
                    }
                    
                    found_any = 1;
                    item_type = current_item;
                    total_removed += removed;
                    total_points += points;
                    break;
                }
                
                /* 5. Groupe de 3+ (simple) */
                pos_count = detect_group_3_plus(board, row, col, temp_positions);
                if (pos_count >= 3) {
                    points = pos_count; /* 1 point par item */
                    
                    /* Supprimer les items */
                    for (i = 0; i < pos_count; i++) {
                        if (board[temp_positions[i].row][temp_positions[i].col] == current_item) {
                            board[temp_positions[i].row][temp_positions[i].col] = ' ';
                            removed++;
                        }
                    }
                    
                    /* Mettre à jour le contrat */
                    char items[] = {SUN, STRAWBERRY, APPLE, ONION, MANDARIN};
                    int item_idx = -1;
                    for (i = 0; i < NUM_ITEMS; i++) {
                        if (items[i] == current_item) {
                            item_idx = i;
                            break;
                        }
                    }
                    if (item_idx >= 0) {
                        contract_counts[item_idx] += removed;
                    }
                    
                    found_any = 1;
                    item_type = current_item;
                    total_removed += removed;
                    total_points += points;
                    break;
                }
            }
            
            if (found_any) {
                break; /* Sortir de la boucle pour réappliquer la gravité */
            }
        }
        
        /* Appliquer la gravité après chaque suppression */
        if (found_any) {
            apply_gravity(board);
        }
    }
    
    result.count = total_removed;
    result.points = total_points;
    result.item_type = item_type;
    
    return result;
}

