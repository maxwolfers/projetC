#include "contract.h"
#include <stdlib.h>
#include <time.h>

/**
 * Génère un contrat pour un niveau donné
 */
LevelContract generate_contract(int level) {
    LevelContract contract;
    int i;
    
    /* Initialiser tous les objectifs à zéro */
    for (i = 0; i < NUM_ITEMS; i++) {
        contract.items[i] = 0;
    }
    
    /* Générer des objectifs selon le niveau */
    int num_objectives = 2 + (level / 2); /* 2-7 objectifs selon le niveau */
    if (num_objectives > NUM_ITEMS) {
        num_objectives = NUM_ITEMS;
    }
    
    int used[NUM_ITEMS] = {0};
    
    for (i = 0; i < num_objectives; i++) {
        int item_idx;
        do {
            item_idx = rand() % NUM_ITEMS;
        } while (used[item_idx]);
        
        used[item_idx] = 1;
        
        /* Objectif proportionnel au niveau */
        int base_target = 15 + (level * 5);
        int variation = rand() % (base_target / 2);
        contract.items[item_idx] = base_target + variation;
    }
    
    /* Nombre de coups et temps selon la difficulté */
    contract.max_moves = 25 + (level * 5);
    contract.time_limit = 60 + (level * 10);
    
    return contract;
}

/**
 * Vérifie si le contrat est rempli
 */
int is_contract_complete(LevelContract contract, int counts[]) {
    int i;
    for (i = 0; i < NUM_ITEMS; i++) {
        if (contract.items[i] > 0 && counts[i] < contract.items[i]) {
            return 0;
        }
    }
    return 1;
}

/**
 * Met à jour les compteurs de contrat après suppression d'items
 */
void update_contract_counts(char item, int count, LevelContract contract, int counts[]) {
    int item_idx = -1;
    char items[] = {SUN, STRAWBERRY, APPLE, ONION, MANDARIN};
    int i;
    
    for (i = 0; i < NUM_ITEMS; i++) {
        if (items[i] == item) {
            item_idx = i;
            break;
        }
    }
    
    if (item_idx >= 0 && contract.items[item_idx] > 0) {
        counts[item_idx] += count;
        if (counts[item_idx] > contract.items[item_idx]) {
            counts[item_idx] = contract.items[item_idx]; /* Ne pas dépasser */
        }
    }
}

/**
 * Initialise les compteurs à zéro
 */
void init_contract_counts(int counts[]) {
    int i;
    for (i = 0; i < NUM_ITEMS; i++) {
        counts[i] = 0;
    }
}

