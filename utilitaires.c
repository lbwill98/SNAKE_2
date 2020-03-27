#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <pthread.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

#include "utilitaires.h"

void initCarte(Carte *carte, char * fichier)
{
    FILE *f;
    char str[100];
    int i,j;
    char ch;
    carte->lignes = 0;
    f = fopen(fichier,"r");
    if (f == NULL)
    {
        printf("Je ne peux pas ouvrir le fichier %s\n",fichier);
        exit(-1);
    }
    while (fgets(str,sizeof(str),f) != NULL)
    {
        str[strlen(str)-1] = '\0';
        carte->colonnes = strlen(str);
        carte->lignes++;
    }
    fclose(f);

    carte->plateau = (int **)malloc(carte->lignes*sizeof(int *));
    for (i=0; i < carte->lignes; i++)
    {
        carte->plateau[i] = (int *)malloc(carte->colonnes*sizeof(int));
    }

    f = fopen(fichier,"r");
    i = 0;
    j = 0;
    while ((ch = getc(f)) != EOF)
    {
        if (ch != '\n')
        {
            if (ch == ' ')
            {
                carte->plateau[i][j] = VIDE;
            }
            else
            {
                carte->plateau[i][j] = (int)ch-48;
                if((int)ch==TETEV+48)
                {
                    carte->snakeV.head[1]=i;
                    carte->snakeV.head[2]=j;
                }
                if((int)ch==QUEUEV+48)
                {
                    carte->snakeV.tail[1]=i;
                    carte->snakeV.tail[2]=j;
                }
            }
            j++;
            if (j == carte->colonnes)
            {
                j = 0;
                i++;
            }
        }
    }
    fclose(f);

    carte->snakeV.length = 0;
    carte->snakeV.body = (int **)malloc(3*sizeof(int *));
    for (i=0; i < 3; i++)
    {
        carte->snakeV.body[i] = (int *)malloc(LENGTHMAX*sizeof(int));
    }

    if(carte->snakeV.head[1]==carte->snakeV.tail[1] && carte->snakeV.head[2]==carte->snakeV.tail[2]+1)
    {
        carte->snakeV.head[0]=DROITE;
        carte->snakeV.tail[0]=DROITE;
    }
    if(carte->snakeV.head[1]==carte->snakeV.tail[1] && carte->snakeV.head[2]==carte->snakeV.tail[2]-1)
    {
        carte->snakeV.head[0]=GAUCHE;
        carte->snakeV.tail[0]=GAUCHE;
    }
    if(carte->snakeV.head[1]==carte->snakeV.tail[1]+1 && carte->snakeV.head[2]==carte->snakeV.tail[2])
    {
        carte->snakeV.head[0]=BAS;
        carte->snakeV.tail[0]=BAS;
    }
    if(carte->snakeV.head[1]==carte->snakeV.tail[1]-1 && carte->snakeV.head[2]==carte->snakeV.tail[2])
    {
        carte->snakeV.head[0]=HAUT;
        carte->snakeV.tail[0]=HAUT;
    }
}
