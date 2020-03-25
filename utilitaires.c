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
    carte->lignes = 5;
    carte->colonnes=5;
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
            if (ch == '#')
            {
                carte->plateau[i][j] = CORPV;
            }
            else
            {
                carte->plateau[i][j] = VIDE;
            }
            if (ch == '@')
            {
                carte->plateau[i][j] = TETEV;
                //sserpentV->positionTete =i,j
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
}
