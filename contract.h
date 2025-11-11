#ifndef CONTRACT_H
#define CONTRACT_H

#include "config.h"

/* Structure pour un contrat de niveau */
typedef struct {
    int items[NUM_ITEMS];   /* Objectifs pour chaque type (S, F, P, O, M) */
    int max_moves;          /* Nombre maximum de coups */
    int time_limit;         /* Temps limite en secondes */
} LevelContract;

/**
 * Génère un contrat pour un niveau donné
 * @param level: niveau (1, 2, 3, ...) (IN)
 * @return structure LevelContract avec les objectifs (OUT)
 */
LevelContract generate_contract(int level);

/**
 * Vérifie si le contrat est rempli
 * @param contract: contrat actuel (IN)
 * @param counts: compteurs actuels (IN)
 * @return 1 si rempli, 0 sinon (OUT)
 */
int is_contract_complete(LevelContract contract, int counts[]);

/**
 * Met à jour les compteurs de contrat après suppression d'items
 * @param item: type d'item supprimé (IN)
 * @param count: nombre d'items supprimés (IN)
 * @param contract: contrat actuel (IN)
 * @param counts: compteurs à mettre à jour (IN/OUT)
 */
void update_contract_counts(char item, int count, LevelContract contract, int counts[]);

/**
 * Initialise les compteurs à zéro
 * @param counts: tableau des compteurs (OUT)
 */
void init_contract_counts(int counts[]);

#endif /* CONTRACT_H */

