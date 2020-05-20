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

enum {HAUT,BAS,GAUCHE,DROITE,VIRAGEHG,VIRAGEHD,VIRAGEBG,VIRAGEBD};//direction
enum {VIDE,MUR,TETEV,CORPV,QUEUEV,TETER,CORPR,QUEUER,FRUIT};//type de surface

typedef struct
{
    int length;
    int head[4];//[0] direction //[1] ligne // [2] colonne //[3] direction precedente
    int **body;//[0] direction //[1] ligne // [2] colonne
    int tail[6];//[0] direction //[1] ligne // [2] colonne //[3]direction precedente // [4] ligne precedente // [5] colonne precedente
} Snake;

typedef struct
{
    int jouer;
    int **plateau;
    int lignes;
    int colonnes;
    int fruit;
    Snake snakeV;
    Snake snakeR;
} Carte;

void initCarte(Carte *carte, char * fichier);
void placer_fruit(Carte *carte);
int my_rand (int maximum);

#endif // UTILITAIRES_H_INCLUDED
