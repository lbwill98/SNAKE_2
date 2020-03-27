#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <pthread.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

#include "jeu.h"



SDL_Rect position;
SDL_Rect positionTeteV;
int continuerJeu;


void jouer(SDL_Surface* ecran)
{
    Touches etat_clavier;
    memset(&etat_clavier,0,sizeof(etat_clavier));

    SDL_Surface *teteV[4]= {NULL};
    SDL_Surface *teteActuelleV = NULL;
    SDL_Surface *corp =  NULL;
    SDL_Surface *fond = NULL;
    SDL_Event event;

    Carte carte;
    initCarte(&carte,"plateauB20X30.txt");


    fond = IMG_Load("plateauBlanc2030.png");
    teteV[BAS] = IMG_Load("teteVB.png");
    teteV[GAUCHE] = IMG_Load("teteVG.png");
    teteV[DROITE] = IMG_Load("teteVD.png");
    teteV[HAUT] = IMG_Load("teteVH.png");
    corp = IMG_Load("corpVB.png");
    int direction =BAS;
    teteActuelleV = teteV[direction];

    positionTeteV.x = 2;
    positionTeteV.y = 2;

    carte.plateau[positionTeteV.y][positionTeteV.x] = TETEV;

    continuerJeu = 1;
    while(continuerJeu==1)
    {
        updateClavier(&etat_clavier);

        if(etat_clavier.key[SDLK_UP])
        {
            direction = HAUT;
        }
        if(etat_clavier.key[SDLK_DOWN])
        {
            direction = BAS;
        }
        if(etat_clavier.key[SDLK_LEFT])
        {
            direction = GAUCHE;
        }
        if(etat_clavier.key[SDLK_RIGHT])
        {
            direction = DROITE;
        }
        if(etat_clavier.key[SDLK_ESCAPE])
        {
            continuerJeu = 0;
        }

        teteActuelleV = teteV[direction];
        deplacer(&carte,&positionTeteV,direction);
        /*SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            continuerJeu=0;
            break;

        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuerJeu = 0;
                break;

            case SDLK_UP:
                teteActuelleV = teteV[HAUT];
                deplacer(&carte,&positionTeteV,HAUT);
                break;

            case SDLK_DOWN:
                teteActuelleV = teteV[BAS];
                deplacer(&carte,&positionTeteV,BAS);
                break;

            case SDLK_LEFT:
                teteActuelleV = teteV[GAUCHE];
                deplacer(&carte,&positionTeteV,GAUCHE);
                break;

            case SDLK_RIGHT:
                teteActuelleV = teteV[DROITE];
                deplacer(&carte,&positionTeteV,DROITE);
                break;
            }
        }*/

        position.x = 0;
        position.y = 0;
        SDL_BlitSurface(fond,NULL,ecran,&position);

        for (int i=0; i<carte.lignes; i++)
        {
            for (int j=0; j<carte.colonnes; j++)
            {
                position.x = j*TAILLE_BLOC;
                position.y = i*TAILLE_BLOC;

                switch(carte.plateau[i][j])
                {
                case VIDE:
                    break;

                case TETEV:
                    SDL_BlitSurface(teteActuelleV,NULL,ecran,&position);
                    break;

                case CORPV:
                    SDL_BlitSurface(corp,NULL,ecran,&position);
                    break;
                }
            }
        }

        SDL_Flip(ecran);
        SDL_Delay(200);
    }

    for(int i=0; i<4; i++)
    {
        SDL_FreeSurface(teteV[i]);
    }
    //SDL_FreeSurface(teteActuelleV);//as beoins , car deja fait avec dans la boucle
    SDL_FreeSurface(corp);
    SDL_FreeSurface(fond);
}



void deplacer(Carte *carte, SDL_Rect *pos, int direction)// ATTENTION BUG AVEC GAUCHE ET DROITE POUR LA TP AVEC DEUX JOUEURS, SI L'UN EST PRES DE LA PORTE ET QUE L'ON FAIT BOUGER L'AUTRE
{
    switch(direction)
    {
    case HAUT:
        if(carte->plateau[pos->y-1][pos->x]!=VIDE)
        {
            //continuerJeu =0;
            break;
        }
        carte->plateau[pos->y][pos->x]=VIDE;
        pos -> y--;
        carte->plateau[pos->y][pos->x]=TETEV;
        break;

    case BAS:
        if(carte->plateau[pos->y+1][pos->x]!=VIDE)
        {
            //continuerJeu =0;
            break;
        }
        carte->plateau[pos->y][pos->x]=VIDE;
        pos -> y++;
        carte->plateau[pos->y][pos->x]=TETEV;
        break;

    case GAUCHE:
        if(carte->plateau[pos->y][pos->x-1]!=VIDE)
        {
            //continuerJeu =0;
            break;
        }
        carte->plateau[pos->y][pos->x]=VIDE;
        pos -> x--;
        carte->plateau[pos->y][pos->x]=TETEV;
        break;

    case DROITE:
        if(carte->plateau[pos->y][pos->x+1]!=VIDE)
        {
            //continuerJeu =0;
            break;
        }
        carte->plateau[pos->y][pos->x]=VIDE;
        pos -> x++;
        carte->plateau[pos->y][pos->x]=TETEV;
        break;
    }
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
