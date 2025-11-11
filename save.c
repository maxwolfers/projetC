#include "save.h"
#include <stdio.h>
#include <string.h>

/**
 * Sauvegarde la progression du joueur
 */
int save_game(SaveData *save_data) {
    FILE *file = fopen(SAVE_FILE, "wb");
    if (file == NULL) {
        return 0;
    }
    
    if (fwrite(save_data, sizeof(SaveData), 1, file) != 1) {
        fclose(file);
        return 0;
    }
    
    fclose(file);
    return 1;
}

/**
 * Charge la progression du joueur
 */
int load_game(SaveData *save_data) {
    FILE *file = fopen(SAVE_FILE, "rb");
    if (file == NULL) {
        return 0;
    }
    
    if (fread(save_data, sizeof(SaveData), 1, file) != 1) {
        fclose(file);
        return 0;
    }
    
    fclose(file);
    return 1;
}

/**
 * Demande le nom du joueur
 */
void get_player_name(char name[], int size) {
    printf("Entrez votre nom: ");
    fgets(name, size, stdin);
    
    /* Supprimer le '\n' final */
    int len = strlen(name);
    if (len > 0 && name[len - 1] == '\n') {
        name[len - 1] = '\0';
    }
    
    /* Si vide, nom par défaut */
    if (strlen(name) == 0) {
        strcpy(name, "Joueur");
    }
}

