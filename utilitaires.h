#ifndef UTILITAIRES_H_INCLUDED
#define UTILITAIRES_H_INCLUDED
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <windows.h>
#include <conio.h>
#define TAILLE_BLOC 32
#define LENGTHMAX 598 //taille maximale 30x20-2

enum {HAUT,BAS,GAUCHE,DROITE};
enum {VIDE,MUR,TETEV,CORPV,VIRAGEV,QUEUEV,TETER,CORPR,VIRAGER,QUEUER,FRUIT};

typedef struct
{
    int key[SDLK_LALT];
} Touches;

typedef struct
{
    int length;
    int head[3];
    int **body;
    int tail[3];
} Snake;

typedef struct
{
    int jouer;
    int **plateau;
    int lignes;
    int colonnes;
    int fruit;
    Snake snakeV;
} Carte;





void initCarte(Carte *carte, char * fichier);
void placer_fruit(Carte *carte);
int my_rand (int maximum);

#endif // UTILITAIRES_H_INCLUDED
