#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <pthread.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

#include "jeu.h"
#include "utilitaires.h"


SDL_Rect position;
SDL_Rect positionTeteV;
SDL_Rect positionFond;

void jouer(SDL_Surface* ecran)
{
    Touches etat_clavier;
    memset(&etat_clavier,0,sizeof(etat_clavier));

    SDL_Surface *teteV[4]= {NULL};
    SDL_Surface *teteActuelleV = NULL;
    SDL_Surface *fond = NULL;
    SDL_Event event;

    int **carte = (int**)malloc(5*sizeof(int*));
    for(int i=0; i<11; i++)
    {
        carte[i] = (int*)malloc(5*sizeof(int));
    }

    carte[0][0]=VIDE;
    carte[0][1]=VIDE;
    carte[0][2]=VIDE;
    carte[0][3]=VIDE;
    carte[0][4]=VIDE;

    carte[1][0]=VIDE;
    carte[1][1]=VIDE;
    carte[1][2]=VIDE;
    carte[1][3]=VIDE;
    carte[1][4]=VIDE;

    carte[2][0]=VIDE;
    carte[2][1]=VIDE;
    carte[2][2]=VIDE;
    carte[2][3]=VIDE;
    carte[2][4]=VIDE;

    carte[3][0]=VIDE;
    carte[3][1]=VIDE;
    carte[3][2]=VIDE;
    carte[3][3]=VIDE;
    carte[3][4]=VIDE;

    carte[4][0]=VIDE;
    carte[4][1]=VIDE;
    carte[4][2]=VIDE;
    carte[4][3]=VIDE;
    carte[4][4]=VIDE;

    fond = IMG_Load("plateauBlanc55.png");
    teteV[BAS] = IMG_Load("teteVB.png");
    teteV[GAUCHE] = IMG_Load("teteVG.png");
    teteV[DROITE] = IMG_Load("teteVD.png");
    teteV[HAUT] = IMG_Load("teteVH.png");
    teteActuelleV = teteV[BAS];

    positionTeteV.x = 2;
    positionTeteV.y = 2;

    //carte[positionTeteV.y][positionTeteV.x] = TETEV;

    int continuer = 1;
    while(continuer==1)
    {
        updateClavier(&etat_clavier);

        if(etat_clavier.key[SDLK_UP])
        {
            teteActuelleV = teteV[HAUT];
            deplacer(carte,&positionTeteV,HAUT);
        }
        if(etat_clavier.key[SDLK_DOWN])
        {
            teteActuelleV = teteV[BAS];
            deplacer(carte,&positionTeteV,BAS);
        }
        if(etat_clavier.key[SDLK_LEFT])
        {
            teteActuelleV = teteV[GAUCHE];
            deplacer(carte,&positionTeteV,GAUCHE);
        }
        if(etat_clavier.key[SDLK_RIGHT])
        {
            teteActuelleV = teteV[DROITE];
            deplacer(carte,&positionTeteV,DROITE);
        }
        if(etat_clavier.key[SDLK_ESCAPE])
        {
            continuer = 0;
        }


        positionFond.x = 0;
        positionFond.y = 0;
        SDL_BlitSurface(fond,NULL,ecran,&positionFond);

        for (int i=0; i<5; i++)
        {
            for (int j=0; j<5; j++)
            {
                position.x = j*TAILLE_BLOC;
                position.y = i*TAILLE_BLOC;

                switch(carte[i][j])
                {
                case VIDE:
                    break;

                case TETEV:
                    SDL_BlitSurface(teteActuelleV,NULL,ecran,&position);
                    break;
                }
            }
        }

        position.x = positionTeteV.x*TAILLE_BLOC;
        position.y = positionTeteV.y*TAILLE_BLOC;
        SDL_BlitSurface(teteActuelleV, NULL, ecran, &position);

        SDL_Flip(ecran);
        SDL_Delay(100);
    }

    for(int i=0; i<4; i++)
    {
        SDL_FreeSurface(teteV[i]);
    }
    SDL_FreeSurface(teteActuelleV);
    SDL_FreeSurface(fond);
}

void updateClavier(Touches *etat_clavier)
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_KEYDOWN:
            etat_clavier -> key[event.key.keysym.sym]=1;
            break;

        case SDL_KEYUP:
            etat_clavier -> key[event.key.keysym.sym]=0;
            break;

        default:
            break;
        }
    }
}

void deplacer(int **carte, SDL_Rect *pos, int direction)// ATTENTION BUG AVEC GAUCHE ET DROITE POUR LA TP AVEC DEUX JOUEURS, SI L'UN EST PRES DE LA PORTE ET QUE L'ON FAIT BOUGER L'AUTRE
{
    switch(direction)
    {
    case HAUT:
        pos -> y--;
        break;

    case BAS:
        pos -> y++;
        break;

    case GAUCHE:
        pos -> x--;
        break;

    case DROITE:
        pos -> x++;
        break;
    }
}
