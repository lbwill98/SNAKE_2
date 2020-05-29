#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <pthread.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

#include "utilitaires.h"

void initCarte(Carte *carte, char * fichier, int speed)
{
    FILE *f;
    char str[100];
    int i,j;
    char ch;
    carte->jouer = 1;
    carte->lignes = 0;
    carte->fruit = 0;
    carte->positionFruit[0]=-1;
    carte->positionFruit[1]=-1;
    carte->speed = speed;
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
                if((int)ch==TETER+48)
                {
                    carte->snakeR.head[1]=i;
                    carte->snakeR.head[2]=j;
                }
                if((int)ch==QUEUER+48)
                {
                    carte->snakeR.tail[1]=i;
                    carte->snakeR.tail[2]=j;
                }
                if((int)ch==FRUIT+48)
                {
                    carte->fruit++;
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
    carte->snakeR.length = 0;

    carte->snakeV.body = (int **)malloc(3*sizeof(int *));
    for (i=0; i < 3; i++)
    {
        carte->snakeV.body[i] = (int *)malloc(LENGTHMAX*sizeof(int));
    }
    carte->snakeR.body = (int **)malloc(3*sizeof(int *));
    for (i=0; i < 3; i++)
    {
        carte->snakeR.body[i] = (int *)malloc(LENGTHMAX*sizeof(int));
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
    if(carte->snakeR.head[1]==carte->snakeR.tail[1] && carte->snakeR.head[2]==carte->snakeR.tail[2]+1)
    {
        carte->snakeR.head[0]=DROITE;
        carte->snakeR.tail[0]=DROITE;
    }
    if(carte->snakeR.head[1]==carte->snakeR.tail[1] && carte->snakeR.head[2]==carte->snakeR.tail[2]-1)
    {
        carte->snakeR.head[0]=GAUCHE;
        carte->snakeR.tail[0]=GAUCHE;
    }
    if(carte->snakeR.head[1]==carte->snakeR.tail[1]+1 && carte->snakeR.head[2]==carte->snakeR.tail[2])
    {
        carte->snakeR.head[0]=BAS;
        carte->snakeR.tail[0]=BAS;
    }
    if(carte->snakeR.head[1]==carte->snakeR.tail[1]-1 && carte->snakeR.head[2]==carte->snakeR.tail[2])
    {
        carte->snakeR.head[0]=HAUT;
        carte->snakeR.tail[0]=HAUT;
    }

    carte->snakeV.head[3]=carte->snakeV.head[0];
    carte->snakeR.head[3]=carte->snakeR.head[0];

    carte->snakeV.tail[3]=carte->snakeV.tail[0];
    carte->snakeR.tail[3]=carte->snakeR.tail[0];
    carte->snakeV.tail[4]=carte->snakeV.tail[1];
    carte->snakeR.tail[4]=carte->snakeR.tail[1];
    carte->snakeV.tail[5]=carte->snakeV.tail[2];
    carte->snakeR.tail[5]=carte->snakeR.tail[2];

    /*if(carte->fruit==0)//si pas de fruit dans le fichier
    {
        placer_fruit(carte);
    }*/
}

void placer_fruit(Carte *carte)
{
    int placer = 0;
    while(!placer)
    {
        int i = my_rand(carte->lignes);
        int j = my_rand(carte->colonnes);
        if(carte->plateau[i][j]==VIDE)
        {
            carte->plateau[i][j] = FRUIT;
            carte->fruit++;
            carte->positionFruit[0]=i;
            carte->positionFruit[1]=j;
            placer = 1;
        }
    }
}

int my_rand (int maximum)
{
    int res;
    int i;
    for (i = 0; i<2; i++)
    {
        static int first = 0;
        if (first == 0)
        {
            srand (time (NULL));
            first = 1;
        }
        res =(int)(rand()/(double)RAND_MAX*(maximum));
    }
    return (res);
}
