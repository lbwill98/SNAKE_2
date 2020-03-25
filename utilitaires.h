#ifndef UTILITAIRES_H_INCLUDED
#define UTILITAIRES_H_INCLUDED

#define TAILLE_BLOC 32

enum {HAUT,BAS,GAUCHE,DROITE};
enum {VIDE,TETEV,CORPV};

typedef struct
{
    int key[SDLK_LALT]
} Touches;

typedef struct
{
    int **plateau;
    int lignes;
    int colonnes;
} Carte;

void initCarte(Carte *carte, char * fichier);

#endif // UTILITAIRES_H_INCLUDED
