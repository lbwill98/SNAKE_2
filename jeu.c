#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <pthread.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

#include "jeu.h"



SDL_Rect position;
int continuerJeu;

void jouer(SDL_Surface* ecran)
{
    Carte carte;
    initCarte(&carte,"plateauB20X30.txt");


    Touches etat_clavier;
    memset(&etat_clavier,0,sizeof(etat_clavier));

    SDL_Surface *teteV[4]= {NULL};
    SDL_Surface *teteActuelleV = NULL;
    SDL_Surface *corpV[4] = {NULL};
    SDL_Surface *corpActuelleV =  NULL;
    SDL_Surface *virageV[4] = {NULL};
    SDL_Surface *virageActuelleV =  NULL;
    SDL_Surface *queueV[4] = {NULL};
    SDL_Surface *queueActuelleV = NULL;
    SDL_Surface *fond = NULL;
    SDL_Surface *mur = NULL;
    SDL_Event event;

    teteV[HAUT] = IMG_Load("teteVH.png");
    teteV[BAS] = IMG_Load("teteVB.png");
    teteV[GAUCHE] = IMG_Load("teteVG.png");
    teteV[DROITE] = IMG_Load("teteVD.png");
    teteActuelleV = teteV[carte.snakeV.head[0]];

    corpV[HAUT] = IMG_Load("corpVH.png");
    corpV[BAS] = IMG_Load("corpVB.png");
    corpV[GAUCHE] = IMG_Load("corpVG.png");
    corpV[DROITE] = IMG_Load("corpVD.png");

    virageV[HAUT] = IMG_Load("virageVHG.png");
    virageV[BAS] = IMG_Load("virageVBD.png");
    virageV[GAUCHE] = IMG_Load("virageVBG.png");
    virageV[DROITE] = IMG_Load("virageVHD.png");

    queueV[HAUT] = IMG_Load("queueVH.png");
    queueV[BAS] = IMG_Load("queueVB.png");
    queueV[GAUCHE] = IMG_Load("queueVG.png");
    queueV[DROITE] = IMG_Load("queueVD.png");
    queueActuelleV = queueV[carte.snakeV.tail[0]];

    fond = IMG_Load("plateauBlanc2030.png");
    mur = IMG_Load("mur.png");

    continuerJeu = 1;
    while(continuerJeu==1)
    {

        updateClavier(&etat_clavier);

        if(etat_clavier.key[SDLK_UP])
        {
            carte.snakeV.head[0] = HAUT;
        }
        if(etat_clavier.key[SDLK_DOWN])
        {
            carte.snakeV.head[0] = BAS;
        }
        if(etat_clavier.key[SDLK_LEFT])
        {
            carte.snakeV.head[0] = GAUCHE;
        }
        if(etat_clavier.key[SDLK_RIGHT])
        {
            carte.snakeV.head[0] = DROITE;
        }
        if(etat_clavier.key[SDLK_ESCAPE])
        {
            continuerJeu = 0;
        }

        /*      SDL_WaitEvent(&event);
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
                      carte.snakeV.head[0] = HAUT;
                      break;

                  case SDLK_DOWN:
                      carte.snakeV.head[0] = BAS;
                      break;

                  case SDLK_LEFT:
                      carte.snakeV.head[0] = GAUCHE;
                      break;

                  case SDLK_RIGHT:
                      carte.snakeV.head[0] = DROITE;
                      break;
                  }
              }
        */
        teteActuelleV = teteV[carte.snakeV.head[0]];
        deplacer(&carte);
        queueActuelleV = queueV[carte.snakeV.tail[0]];

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

                case MUR:
                    SDL_BlitSurface(mur,NULL,ecran,&position);
                    break;

                case TETEV:
                    SDL_BlitSurface(teteActuelleV,NULL,ecran,&position);
                    break;

                case CORPV:
                    SDL_BlitSurface(corpActuelleV,NULL,ecran,&position);
                    break;

                case VIRAGEV:
                    SDL_BlitSurface(virageActuelleV,NULL,ecran,&position);
                    break;

                case QUEUEV:
                    SDL_BlitSurface(queueActuelleV,NULL,ecran,&position);
                    break;
                }
            }
        }
        SDL_Flip(ecran);
        SDL_Delay(100);
        //continuerJeu =0;
    }

    for(int i=0; i<4; i++)
    {
        SDL_FreeSurface(teteV[i]);
        SDL_FreeSurface(corpV[i]);
        SDL_FreeSurface(virageV[i]);
        SDL_FreeSurface(queueV[i]);
    }
    //SDL_FreeSurface(teteActuelleV);//as beoins , car deja fait avec dans la boucle
    SDL_FreeSurface(fond);
}



void deplacer(Carte *carte)// ATTENTION BUG AVEC GAUCHE ET DROITE POUR LA TP AVEC DEUX JOUEURS, SI L'UN EST PRES DE LA PORTE ET QUE L'ON FAIT BOUGER L'AUTRE
{
    carte->plateau[carte->snakeV.tail[1]][carte->snakeV.tail[2]]=VIDE;// a delacer a la fin de la fonction en gardant en memoire les indices de la queue au debut de fonction pour que le serpent ne passe pas sur sa queue
    if(carte->snakeV.length == 0)
    {
        carte->snakeV.tail[1]=carte->snakeV.head[1];
        carte->snakeV.tail[2]=carte->snakeV.head[2];
        carte->plateau[carte->snakeV.tail[1]][carte->snakeV.tail[2]]=QUEUEV;
        //sur un mur, le serpent s'arrete mais la queue prnd la place de la tete...
        switch(carte->snakeV.head[0])
        {
        case HAUT:
            carte->snakeV.tail[0]=HAUT;
            if(carte->plateau[carte->snakeV.head[1]-1][carte->snakeV.head[2]]!=VIDE)
            {
                //continuerJeu =0;
                break;
            }
            carte->snakeV.head[1]--;
            carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]]=TETEV;
            break;

        case BAS:
            carte->snakeV.tail[0]=BAS;
            if(carte->plateau[carte->snakeV.head[1]+1][carte->snakeV.head[2]]!=VIDE)
            {
                //continuerJeu =0;
                break;
            }
            carte->snakeV.head[1]++;
            carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]]=TETEV;
            break;

        case GAUCHE:
            carte->snakeV.tail[0]=GAUCHE;
            if(carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]-1]!=VIDE)
            {
                //continuerJeu =0;
                break;
            }
            carte->snakeV.head[2]--;
            carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]]=TETEV;
            break;

        case DROITE:
            carte->snakeV.tail[0]=DROITE;
            if(carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]+1]!=VIDE)
            {
                //continuerJeu =0;
                break;
            }
            carte->snakeV.head[2]++;
            carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]]=TETEV;
            break;
        }
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
