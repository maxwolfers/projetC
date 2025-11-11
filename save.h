#ifndef SAVE_H
#define SAVE_H

#include "config.h"

/* Structure de sauvegarde */
typedef struct {
    char player_name[50];
    int level;
    int lives;
    int completed_levels[MAX_LEVELS];
} SaveData;

/**
 * Sauvegarde la progression du joueur
 * @param save_data: données à sauvegarder (IN)
 * @return 1 si succès, 0 sinon (OUT)
 */
int save_game(SaveData *save_data);

/**
 * Charge la progression du joueur
 * @param save_data: structure à remplir (OUT)
 * @return 1 si succès, 0 sinon (OUT)
 */
int load_game(SaveData *save_data);

/**
 * Demande le nom du joueur
 * @param name: buffer pour stocker le nom (OUT)
 * @param size: taille du buffer (IN)
 */
void get_player_name(char name[], int size);

#endif /* SAVE_H */

