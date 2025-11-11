#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "config.h"
#include "game.h"
#include "display.h"
#include "save.h"
#include "input.h"

/**
 * Fonction principale du programme
 */
int main(void) {
    int choice;
    GameState game;
    SaveData save_data;
    int game_result;
    int continue_game = 1;
    
    /* Initialiser le générateur aléatoire */
    srand((unsigned int)time(NULL));
    
    while (continue_game) {
        /* Afficher le menu */
        choice = display_menu();
        
        switch (choice) {
            case 1: /* Nouvelle partie */
                /* Demander le nom du joueur */
                get_player_name(save_data.player_name, sizeof(save_data.player_name));
                
                /* Initialiser le jeu au niveau 1 */
                init_game_state(&game, 1);
                strcpy(game.save_data.player_name, save_data.player_name);
                game.save_data.level = 1;
                game.save_data.lives = INITIAL_LIVES;
                
                /* Boucle de niveaux */
                while (game.lives > 0) {
                    /* Jouer le niveau */
                    game_result = game_loop(&game);
                    
                    if (game_result == 1) {
                        /* Victoire */
                        display_victory(game.level);
                        printf("Appuyez sur Entrée pour continuer...\n");
                        wait_key();
                        
                        /* Passer au niveau suivant */
                        game.level++;
                        game.save_data.level = game.level;
                        game.lives = INITIAL_LIVES; /* Réinitialiser les vies */
                        game.save_data.lives = game.lives;
                        
                        /* Initialiser le nouveau niveau */
                        init_game_state(&game, game.level);
                        strcpy(game.save_data.player_name, save_data.player_name);
                    } else if (game_result == 0) {
                        /* Défaite */
                        display_defeat();
                        game.lives--;
                        game.save_data.lives = game.lives;
                        
                        if (game.lives > 0) {
                            printf("Vies restantes: %d\n", game.lives);
                            printf("1. Recommencer le niveau\n");
                            printf("2. Quitter\n");
                            printf("Votre choix: ");
                            
                            int retry_choice;
                            scanf("%d", &retry_choice);
                            
                            if (retry_choice == 1) {
                                /* Recommencer le même niveau */
                                init_game_state(&game, game.level);
                                strcpy(game.save_data.player_name, save_data.player_name);
                            } else {
                                /* Quitter */
                                game.lives = 0;
                            }
                        } else {
                            display_game_over();
                            printf("Appuyez sur Entrée pour continuer...\n");
                            wait_key();
                        }
                    } else {
                        /* Quitte */
                        break;
                    }
                }
                
                /* Sauvegarder la progression */
                if (game_result != -1) {
                    save_game(&game.save_data);
                }
                break;
                
            case 2: /* Continuer la partie sauvegardée */
                if (load_game(&save_data)) {
                    printf("Partie chargée pour %s (Niveau %d, %d vies)\n",
                           save_data.player_name, save_data.level, save_data.lives);
                    
                    /* Initialiser le jeu au niveau sauvegardé */
                    init_game_state(&game, save_data.level);
                    game.save_data = save_data;
                    game.lives = save_data.lives;
                    
                    /* Boucle de niveaux */
                    while (game.lives > 0) {
                        game_result = game_loop(&game);
                        
                        if (game_result == 1) {
                            display_victory(game.level);
                            printf("Appuyez sur Entrée pour continuer...\n");
                            wait_key();
                            
                            game.level++;
                            game.save_data.level = game.level;
                            game.lives = INITIAL_LIVES;
                            game.save_data.lives = game.lives;
                            
                            init_game_state(&game, game.level);
                            strcpy(game.save_data.player_name, save_data.player_name);
                        } else if (game_result == 0) {
                            display_defeat();
                            game.lives--;
                            game.save_data.lives = game.lives;
                            
                            if (game.lives > 0) {
                                printf("Vies restantes: %d\n", game.lives);
                                printf("1. Recommencer le niveau\n");
                                printf("2. Quitter\n");
                                printf("Votre choix: ");
                                
                                int retry_choice;
                                scanf("%d", &retry_choice);
                                
                                if (retry_choice == 1) {
                                    init_game_state(&game, game.level);
                                    strcpy(game.save_data.player_name, save_data.player_name);
                                } else {
                                    game.lives = 0;
                                }
                            } else {
                                display_game_over();
                                printf("Appuyez sur Entrée pour continuer...\n");
                                wait_key();
                            }
                        } else {
                            break;
                        }
                    }
                    
                    if (game_result != -1) {
                        save_game(&game.save_data);
                    }
                } else {
                    printf("Aucune partie sauvegardée trouvée.\n");
                    printf("Appuyez sur Entrée pour continuer...\n");
                    wait_key();
                }
                break;
                
            case 3: /* Quitter */
                continue_game = 0;
                break;
                
            default:
                printf("Choix invalide.\n");
                printf("Appuyez sur Entrée pour continuer...\n");
                wait_key();
                break;
        }
    }
    
    printf("Merci d'avoir joué !\n");
    return 0;
}

