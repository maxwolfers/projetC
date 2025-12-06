# Plantamitz - Jeu de Match-3 en Console

## Description

Plantamitz est un jeu de type "match-3" inspiré de FarmHeroes, adapté pour fonctionner en mode console. Le but est d'aligner des items pour les faire disparaître et remplir des contrats dans un nombre limité de coups et de temps.

## Caractéristiques

- Plateau de jeu 10 x 10 avec 5 types d'items colorés (Soleil, Fraise, Pomme, Oignon, Mandarine)
- Système de contrats avec objectifs variés par niveau
- Détection de groupes : 3+, 4, 6, H, rectangle
- Système de gravité automatique
- Gestion du temps et des coups
- Système de vies (5 vies maximum)
- Sauvegarde de progression
- Interface console avec emojis ANSI




# Compilation

Le projet utilise un Makefile pour la compilation :


## MAC

```bash
make
```

Cela créera l'exécutable `Plantamitz`.

Pour nettoyer les fichiers compilés :

```bash
make clean
```

Pour recompiler depuis le début :

```bash
make rebuild
```

### Utilisation

Lancer le jeu :

```bash
./Plantamitz
```


## Windows

```bash
wingw32-make
```

Cela créera l'exécutable `Plantamitz.exe`.

Pour nettoyer les fichiers compilés :

```bash
wingw32-make clean
```

### Utilisation

Lancer le jeu :

```bash
.\Plantamitz.exe
```



### Contrôles

- **flèche haut** : Déplacer le curseur vers le haut
- **flèche bas** : Déplacer le curseur vers le bas
- **flèche gauche** : Déplacer le curseur vers la gauche
- **flèche droite** : Déplacer le curseur vers la droite
- **ESPACE** : Sélectionner/désélectionner un item, puis échanger avec un item adjacent
- **q** : Quitter le jeu
- **s** : sauvegarder la partie
- **m** : activer / désactiver le son

### Règles du jeu

1. **Objectif** : Remplir le contrat en "mangeant" le nombre requis d'items de chaque type
2. **Formation de groupes** : Échangez deux items adjacents pour former des groupes de 3+ items identiques
3. **Types de groupes** :
   - **3+ items** : 1 point par item
   - **4 items** : 4 points
   - **6 items** : Supprime tous les items identiques sur le plateau
   - **H** : 2 × nombre d'items
   - **Rectangle** : 2 × (longueur × largeur)
4. **Gravité** : Les items tombent automatiquement après suppression
5. **Vies** : Vous avez 5 vies. Chaque échec consomme une vie.

## Structure du projet

```
.
├── interface.h/c          # toute la partie visible par l'utilisateur
├── logique.h/c          # Logique principale du jeu
├── main.c            # Programme principal
├── audio.h/c          # gestion d'audio
├── README.md          # détails du projets
├── musique.wav          # l'extrait audio du jeu
└── Makefile          # Fichier de compilation
```

## Fichier de sauvegarde

Le jeu sauvegarde automatiquement la progression dans le fichier `savegame.dat`. Ce fichier contient :
- Le niveau actuel
- L'avancement des contrats
- Le plateau du jeu

## Notes techniques

- Le jeu nécessite un terminal compatible ANSI pour les emojis
- Compatible avec les systèmes Windows
- Utilise les codes de couleur ANSI pour l'affichage

## Auteur

Projet réalisé dans le cadre d'un cours de programmation en C.

