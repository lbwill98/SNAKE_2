#ifndef UTILITAIRES_H_INCLUDED
#define UTILITAIRES_H_INCLUDED

#define TAILLE_BLOC 32

enum {HAUT,BAS,GAUCHE,DROITE};
enum {VIDE,TETEV,CORPV};

typedef struct
{
    int key[SDLK_LALT];
} Touches;

typedef struct
{
    int **plateau;
    int lignes;
    int colonnes;
} Carte;

typedef struct
{
    int length;
    int head[3];
    int **body;
    int tail[3];
} Snake;

void initCarte(Carte *carte, char * fichier);
void initSnake(Snake *snake, Carte *carte, int *head, int *tail);

#endif // UTILITAIRES_H_INCLUDED
