#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <pthread.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

#include "jeu.h"


void jouer(SDL_Surface* ecran)
{
    Carte carte;
    initCarte(&carte,"plateauB20X30.txt");

    SDL_Rect position;
    Touches etat_clavier;
    memset(&etat_clavier,0,sizeof(etat_clavier));
    SDL_Event event;

    SDL_Surface *teteV[4]= {NULL};
    SDL_Surface *teteActuelleV = NULL;
    SDL_Surface *corpV[8] = {NULL};
    SDL_Surface *corpActuelleV =  NULL;
    SDL_Surface *queueV[4] = {NULL};
    SDL_Surface *queueActuelleV = NULL;
    SDL_Surface *fond = NULL;
    SDL_Surface *mur = NULL;
    SDL_Surface *fruit = NULL;


    teteV[HAUT] = IMG_Load("teteVH.png");
    teteV[BAS] = IMG_Load("teteVB.png");
    teteV[GAUCHE] = IMG_Load("teteVG.png");
    teteV[DROITE] = IMG_Load("teteVD.png");
    teteActuelleV = teteV[carte.snakeV.head[0]];

    corpV[HAUT] = IMG_Load("corpVH.png");
    corpV[BAS] = IMG_Load("corpVB.png");
    corpV[GAUCHE] = IMG_Load("corpVG.png");
    corpV[DROITE] = IMG_Load("corpVD.png");
    corpV[VIRAGEHG] = IMG_Load("virageVHG.png");
    corpV[VIRAGEHD] = IMG_Load("virageVHD.png");
    corpV[VIRAGEBG] = IMG_Load("virageVBG.png");
    corpV[VIRAGEBD] = IMG_Load("virageVBD.png");

    queueV[HAUT] = IMG_Load("queueVH.png");
    queueV[BAS] = IMG_Load("queueVB.png");
    queueV[GAUCHE] = IMG_Load("queueVG.png");
    queueV[DROITE] = IMG_Load("queueVD.png");
    queueActuelleV = queueV[carte.snakeV.tail[0]];

    fond = IMG_Load("plateauBlanc2030.png");
    mur = IMG_Load("mur.png");
    fruit = IMG_Load("fruitOr.png");

    while(carte.jouer==1)
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
            carte.jouer = 0;
        }
        /*
                SDL_WaitEvent(&event);
                switch(event.type)
                {
                case SDL_QUIT:
                    carte.jouer=0;
                    break;

                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                    case SDLK_ESCAPE:
                        carte.jouer = 0;
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

                case FRUIT:
                    SDL_BlitSurface(fruit,NULL,ecran,&position);
                    break;

                case TETEV:
                    SDL_BlitSurface(teteActuelleV,NULL,ecran,&position);
                    break;

                case CORPV:
                    for(int k=0;k<carte.snakeV.length;k++)
                    {
                        if(carte.snakeV.body[1][k]==i && carte.snakeV.body[2][k]==j)
                        {
                            corpActuelleV=corpV[carte.snakeV.body[0][k]];
                            SDL_BlitSurface(corpActuelleV,NULL,ecran,&position);
                            break;
                        }
                    }
                    break;
                case QUEUEV:
                    SDL_BlitSurface(queueActuelleV,NULL,ecran,&position);
                    break;
                }
            }
        }
        if(carte.fruit==0)
        {
            placer_fruit(&carte);
        }
        SDL_Flip(ecran);
        SDL_Delay(200);
        //carte.jouer=0;
    }

    for(int i=0; i<4; i++)
    {
        SDL_FreeSurface(teteV[i]);
        SDL_FreeSurface(queueV[i]);
    }
    for(int i=0; i<8; i++)
    {
        SDL_FreeSurface(corpV[i]);
    }
    //SDL_FreeSurface(teteActuelleV);//as beoins , car deja fait avec dans la boucle
    SDL_FreeSurface(fond);
}



void deplacer(Carte *carte)
{
    switch(carte->snakeV.head[0])
    {
    case HAUT:
        if(carte->plateau[carte->snakeV.head[1]-1][carte->snakeV.head[2]]==FRUIT)
        {
            ajouterCorp(carte);
            carte->snakeV.head[1]--;
            carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]]=TETEV;
            break;
        }
        if(carte->plateau[carte->snakeV.head[1]-1][carte->snakeV.head[2]]!=VIDE)//ou queue a ajouter
        {
            //carte.jouer =0;
            break;
        }
        avancerQueue(carte);
        avancerCorp(carte);
        carte->snakeV.head[1]--;
        carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]]=TETEV;
        break;

    case BAS:
        if(carte->plateau[carte->snakeV.head[1]+1][carte->snakeV.head[2]]==FRUIT)
        {
            ajouterCorp(carte);
            carte->snakeV.head[1]++;
            carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]]=TETEV;
            break;
        }
        if(carte->plateau[carte->snakeV.head[1]+1][carte->snakeV.head[2]]!=VIDE)//ou queue a ajouter
        {
            //carte.jouer =0;
            break;
        }
        avancerQueue(carte);
        avancerCorp(carte);
        carte->snakeV.head[1]++;
        carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]]=TETEV;
        break;

    case GAUCHE:
        if(carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]-1]==FRUIT)
        {
            ajouterCorp(carte);
            carte->snakeV.head[2]--;
            carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]]=TETEV;
            break;
        }
        if(carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]-1]!=VIDE)//ou queue a ajouter
        {
            //carte.jouer =0;
            break;
        }
        avancerQueue(carte);
        avancerCorp(carte);
        carte->snakeV.head[2]--;
        carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]]=TETEV;
        break;

    case DROITE:
        if(carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]+1]==FRUIT)
        {
            ajouterCorp(carte);
            carte->snakeV.head[2]++;
            carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]]=TETEV;
            break;
        }
        if(carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]+1]!=VIDE)//ou queue a ajouter
        {
            //carte.jouer =0;
            break;
        }
        avancerQueue(carte);
        avancerCorp(carte);
        carte->snakeV.head[2]++;
        carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]]=TETEV;
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

void ajouterCorp(Carte *carte)
{
    carte->fruit--;
    int directionPrecedente = carte->snakeV.tail[0];
    if(carte->snakeV.length!=0)
    {
        directionPrecedente = carte->snakeV.body[0][carte->snakeV.length-1];
    }
    carte->snakeV.body[1][carte->snakeV.length]=carte->snakeV.head[1];

    carte->snakeV.length++;
    directionPremierCorp(carte,directionPrecedente);
    carte->snakeV.body[2][carte->snakeV.length-1]=carte->snakeV.head[2];
    carte->plateau[carte->snakeV.body[1][carte->snakeV.length-1]][carte->snakeV.body[2][carte->snakeV.length-1]]=CORPV;


}

void avancerCorp(Carte *carte)
{
    for(int j=0; j<carte->snakeV.length-1; j++)
    {
        carte->snakeV.body[0][j]=carte->snakeV.body[0][j+1];
        carte->snakeV.body[1][j]=carte->snakeV.body[1][j+1];
        carte->snakeV.body[2][j]=carte->snakeV.body[2][j+1];
    }
    if(carte->snakeV.length>0)
    {
        if(carte->snakeV.body[0][carte->snakeV.length-1]!=carte->snakeV.head[0])
        {
            directionPremierCorp(carte, carte->snakeV.body[0][carte->snakeV.length-1]);
        }
        carte->snakeV.body[1][carte->snakeV.length-1]=carte->snakeV.head[1];
        carte->snakeV.body[2][carte->snakeV.length-1]=carte->snakeV.head[2];
        carte->plateau[carte->snakeV.body[1][carte->snakeV.length-1]][carte->snakeV.body[2][carte->snakeV.length-1]]=CORPV;
    }
}

void avancerQueue(Carte *carte)
{
    carte->plateau[carte->snakeV.tail[1]][carte->snakeV.tail[2]]=VIDE;
    if(carte->snakeV.length>0)
{
    directionQueue(carte, carte->snakeV.body[0][0]);
        carte->snakeV.tail[1]=carte->snakeV.body[1][0];
        carte->snakeV.tail[2]=carte->snakeV.body[2][0];

    }
    else
    {
        directionQueue(carte, carte->snakeV.head[0]);
        carte->snakeV.tail[1]=carte->snakeV.head[1];
        carte->snakeV.tail[2]=carte->snakeV.head[2];
    }
    carte->plateau[carte->snakeV.tail[1]][carte->snakeV.tail[2]]=QUEUEV;
}

void directionPremierCorp(Carte *carte, int directionPrecedente)
{
    switch(carte->snakeV.head[0])
    {
    case HAUT:
        switch(directionPrecedente)
        {
        case HAUT:
            carte->snakeV.body[0][carte->snakeV.length-1]=HAUT;
            break;
        case GAUCHE:
            carte->snakeV.body[0][carte->snakeV.length-1]=VIRAGEHD;
            break;
        case DROITE:
            carte->snakeV.body[0][carte->snakeV.length-1]=VIRAGEHG;
            break;
        case VIRAGEHG:
            if(carte->snakeV.body[1][carte->snakeV.length-1]==carte->snakeV.head[1])
            {
                carte->snakeV.body[0][carte->snakeV.length-1]=VIRAGEHD;
            }
            else
            {
                carte->snakeV.body[0][carte->snakeV.length-1]=HAUT;
            }
            break;
        case VIRAGEHD:
            if(carte->snakeV.body[1][carte->snakeV.length-1]==carte->snakeV.head[1])
            {
                carte->snakeV.body[0][carte->snakeV.length-1]=VIRAGEHG;
            }
            else
            {
                carte->snakeV.body[0][carte->snakeV.length-1]=HAUT;
            }
            break;
        case VIRAGEBG:
            carte->snakeV.body[0][carte->snakeV.length-1]=VIRAGEHD;
            break;
        case VIRAGEBD:
            carte->snakeV.body[0][carte->snakeV.length-1]=VIRAGEHG;
            break;
        }
        break;
    case BAS:
        switch(directionPrecedente)
        {
        case BAS:
            carte->snakeV.body[0][carte->snakeV.length-1]=BAS;
            break;
        case GAUCHE:
            carte->snakeV.body[0][carte->snakeV.length-1]=VIRAGEBD;
            break;
        case DROITE:
            carte->snakeV.body[0][carte->snakeV.length-1]=VIRAGEBG;
            break;
        case VIRAGEHG:
            carte->snakeV.body[0][carte->snakeV.length-1]=VIRAGEBD;
            break;
        case VIRAGEHD:
            carte->snakeV.body[0][carte->snakeV.length-1]=VIRAGEBG;
            break;
        case VIRAGEBG:
            if(carte->snakeV.body[1][carte->snakeV.length-1]==carte->snakeV.head[1])
            {
                carte->snakeV.body[0][carte->snakeV.length-1]=VIRAGEBD;
            }
            else
            {
                carte->snakeV.body[0][carte->snakeV.length-1]=BAS;
            }
            break;
        case VIRAGEBD:
            if(carte->snakeV.body[1][carte->snakeV.length-1]==carte->snakeV.head[1])
            {
                carte->snakeV.body[0][carte->snakeV.length-1]=VIRAGEBG;
            }
            else
            {
                carte->snakeV.body[0][carte->snakeV.length-1]=BAS;
            }
            break;
        }
        break;
    case GAUCHE:
        switch(directionPrecedente)
        {
        case HAUT:
            carte->snakeV.body[0][carte->snakeV.length-1]=VIRAGEBG;
            break;
        case BAS:
            carte->snakeV.body[0][carte->snakeV.length-1]=VIRAGEHG;
            break;
        case GAUCHE:
            carte->snakeV.body[0][carte->snakeV.length-1]=GAUCHE;
            break;
        case VIRAGEHG:
            if(carte->snakeV.body[1][carte->snakeV.length-1]==carte->snakeV.head[1])
            {
                carte->snakeV.body[0][carte->snakeV.length-1]=GAUCHE;
            }
            else
            {
                carte->snakeV.body[0][carte->snakeV.length-1]=VIRAGEBG;
            }
            break;
        case VIRAGEHD:
            carte->snakeV.body[0][carte->snakeV.length-1]=VIRAGEBG;
            break;
        case VIRAGEBG:
            if(carte->snakeV.body[1][carte->snakeV.length-1]==carte->snakeV.head[1])
            {
                carte->snakeV.body[0][carte->snakeV.length-1]=GAUCHE;
            }
            else
            {
                carte->snakeV.body[0][carte->snakeV.length-1]=VIRAGEHG;
            }
            break;
        case VIRAGEBD:
            carte->snakeV.body[0][carte->snakeV.length-1]=VIRAGEHG;
            break;
        }
        break;
    case DROITE:
        switch(directionPrecedente)
        {
        case HAUT:
            carte->snakeV.body[0][carte->snakeV.length-1]=VIRAGEBD;
            break;
        case BAS:
            carte->snakeV.body[0][carte->snakeV.length-1]=VIRAGEHD;
            break;
        case DROITE:
            carte->snakeV.body[0][carte->snakeV.length-1]=DROITE;
            break;
        case VIRAGEHG:
            carte->snakeV.body[0][carte->snakeV.length-1]=VIRAGEBD;
            break;
        case VIRAGEHD:
            if(carte->snakeV.body[1][carte->snakeV.length-1]==carte->snakeV.head[1])
            {
                carte->snakeV.body[0][carte->snakeV.length-1]=DROITE;
            }
            else
            {
                carte->snakeV.body[0][carte->snakeV.length-1]=VIRAGEBD;
            }
            break;
        case VIRAGEBG:
            carte->snakeV.body[0][carte->snakeV.length-1]=VIRAGEHD;
            break;
        case VIRAGEBD:
            if(carte->snakeV.body[1][carte->snakeV.length-1]==carte->snakeV.head[1])
            {
                carte->snakeV.body[0][carte->snakeV.length-1]=DROITE;
            }
            else
            {
                carte->snakeV.body[0][carte->snakeV.length-1]=VIRAGEHD;
            }
            break;
        }
        break;
    }
}

void directionQueue(Carte *carte, int directionPrecedente)
{
    switch(directionPrecedente)
    {
    case HAUT:
        carte->snakeV.tail[0]=HAUT;
        break;
    case BAS:
        carte->snakeV.tail[0]=BAS;
        break;
    case GAUCHE:
        carte->snakeV.tail[0]=GAUCHE;
        break;
    case DROITE:
        carte->snakeV.tail[0]=DROITE;
        break;
    case VIRAGEHG:
        if(carte->snakeV.tail[0]==BAS)
        {
            carte->snakeV.tail[0]=GAUCHE;
        }
        else
        {
            carte->snakeV.tail[0]=HAUT;
        }
        break;
    case VIRAGEHD:
        if(carte->snakeV.tail[0]==BAS)
        {
            carte->snakeV.tail[0]=DROITE;
        }
        else
        {
            carte->snakeV.tail[0]=HAUT;
        }
        break;
    case VIRAGEBG:
        if(carte->snakeV.tail[0]==HAUT)
        {
            carte->snakeV.tail[0]=GAUCHE;
        }
        else
        {
            carte->snakeV.tail[0]=BAS;
        }
        break;
    case VIRAGEBD:
        if(carte->snakeV.tail[0]==HAUT)
        {
            carte->snakeV.tail[0]=DROITE;
        }
        else
        {
            carte->snakeV.tail[0]=BAS;
        }
        break;
    }

}
