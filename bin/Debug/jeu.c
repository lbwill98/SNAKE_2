#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <pthread.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

#include "jeu.h"

int message=0;

void jouer(SDL_Surface* ecran, int speed)
{
    Carte carte;
    initCarte(&carte,"plateauB20X30.txt", speed); //plateauB20X30 //I_LOVE_ENSEM                                                              //MATIS                                                         //ez //allan

    SDL_Rect position;
    SDL_Event event;

    TTF_Init();
    TTF_Font *police = TTF_OpenFont("BKANT.ttf",24);
    SDL_Color couleurNoir = {0,0,0}, couleurOr = {255,215,0};
    char texteScoreV[20] = "";
    char texteScoreR[20] = "";

    SDL_Surface *teteV[4]= {NULL};
    SDL_Surface *teteActuelleV = NULL;
    SDL_Surface *corpV[8] = {NULL};
    SDL_Surface *corpActuelleV =  NULL;
    SDL_Surface *queueV[4] = {NULL};
    SDL_Surface *queueActuelleV = NULL;
    SDL_Surface *teteR[4]= {NULL};
    SDL_Surface *teteActuelleR = NULL;
    SDL_Surface *corpR[8] = {NULL};
    SDL_Surface *corpActuelleR =  NULL;
    SDL_Surface *queueR[4] = {NULL};
    SDL_Surface *queueActuelleR = NULL;
    SDL_Surface *fond = NULL;
    SDL_Surface *mur = NULL;
    SDL_Surface *fruit = NULL;
    SDL_Surface *scoreV = NULL;
    SDL_Surface *scoreR = NULL;


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

    teteR[HAUT] = IMG_Load("teteRH.png");
    teteR[BAS] = IMG_Load("teteRB.png");
    teteR[GAUCHE] = IMG_Load("teteRG.png");
    teteR[DROITE] = IMG_Load("teteRD.png");
    teteActuelleR = teteR[carte.snakeR.head[0]];

    corpR[HAUT] = IMG_Load("corpRH.png");
    corpR[BAS] = IMG_Load("corpRB.png");
    corpR[GAUCHE] = IMG_Load("corpRG.png");
    corpR[DROITE] = IMG_Load("corpRD.png");
    corpR[VIRAGEHG] = IMG_Load("virageRHG.png");
    corpR[VIRAGEHD] = IMG_Load("virageRHD.png");
    corpR[VIRAGEBG] = IMG_Load("virageRBG.png");
    corpR[VIRAGEBD] = IMG_Load("virageRBD.png");

    queueR[HAUT] = IMG_Load("queueRH.png");
    queueR[BAS] = IMG_Load("queueRB.png");
    queueR[GAUCHE] = IMG_Load("queueRG.png");
    queueR[DROITE] = IMG_Load("queueRD.png");
    queueActuelleR = queueR[carte.snakeR.tail[0]];

    fond = IMG_Load("plateauPierre2030.png");
    mur = IMG_Load("mur.png");
    fruit = IMG_Load("fruitOr.png");

    while(carte.jouer==1)
    {
        carte.snakeV.head[3]=carte.snakeV.head[0];
        carte.snakeR.head[3]=carte.snakeR.head[0];

        //aiV1(&carte);
        //aiR1(&carte);
        //aiV2(&carte);
        aiR3(&carte);

        clock_t start_time = clock();
        while (clock() < start_time + carte.speed)
        {
            SDL_PollEvent(&event);
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

                case SDLK_SPACE:
                    SDL_WaitEvent(&event);
                    break;

                case SDLK_UP:
                    if(carte.plateau[carte.snakeV.head[1]-1][carte.snakeV.head[2]] ==VIDE || carte.plateau[carte.snakeV.head[1]-1][carte.snakeV.head[2]] ==FRUIT)
                    {
                        carte.snakeV.head[0] = HAUT;
                    }
                    break;

                case SDLK_DOWN:
                    if(carte.plateau[carte.snakeV.head[1]+1][carte.snakeV.head[2]] ==VIDE || carte.plateau[carte.snakeV.head[1]+1][carte.snakeV.head[2]] ==FRUIT)
                    {
                        carte.snakeV.head[0] = BAS;
                    }
                    break;

                case SDLK_LEFT:
                    if(carte.plateau[carte.snakeV.head[1]][carte.snakeV.head[2]-1] ==VIDE || carte.plateau[carte.snakeV.head[1]][carte.snakeV.head[2]-1] ==FRUIT)
                    {
                        carte.snakeV.head[0] = GAUCHE;
                    }
                    break;

                case SDLK_RIGHT:
                    if(carte.plateau[carte.snakeV.head[1]][carte.snakeV.head[2]+1] ==VIDE || carte.plateau[carte.snakeV.head[1]][carte.snakeV.head[2]+1] ==FRUIT)
                    {
                        carte.snakeV.head[0] = DROITE;
                    }
                    break;

                case SDLK_e:
                    if(carte.plateau[carte.snakeR.head[1]-1][carte.snakeR.head[2]] ==VIDE || carte.plateau[carte.snakeR.head[1]-1][carte.snakeR.head[2]] ==FRUIT)
                    {
                        carte.snakeR.head[0] = HAUT;
                    }
                    break;

                case SDLK_d:
                    if(carte.plateau[carte.snakeR.head[1]+1][carte.snakeR.head[2]] ==VIDE || carte.plateau[carte.snakeR.head[1]+1][carte.snakeR.head[2]] ==FRUIT)
                    {
                        carte.snakeR.head[0] = BAS;
                    }
                    break;

                case SDLK_s:
                    if(carte.plateau[carte.snakeR.head[1]][carte.snakeR.head[2]-1] ==VIDE || carte.plateau[carte.snakeR.head[1]][carte.snakeR.head[2]-1] ==FRUIT)
                    {
                        carte.snakeR.head[0] = GAUCHE;
                    }
                    break;

                case SDLK_f:
                    if(carte.plateau[carte.snakeR.head[1]][carte.snakeR.head[2]+1] ==VIDE || carte.plateau[carte.snakeR.head[1]][carte.snakeR.head[2]+1] ==FRUIT)
                    {
                        carte.snakeR.head[0] = DROITE;
                    }
                    break;
                }
            }
        }

        if(my_rand(100)>50)//en cas de conflit, on tire au sort !
        {
            teteActuelleV = teteV[carte.snakeV.head[0]];
            deplacerV(&carte);
            queueActuelleV = queueV[carte.snakeV.tail[0]];
            teteActuelleR = teteR[carte.snakeR.head[0]];
            deplacerR(&carte);
            queueActuelleR = queueR[carte.snakeR.tail[0]];
        }
        else
        {
            teteActuelleR = teteR[carte.snakeR.head[0]];
            deplacerR(&carte);
            queueActuelleR = queueR[carte.snakeR.tail[0]];
            teteActuelleV = teteV[carte.snakeV.head[0]];
            deplacerV(&carte);
            queueActuelleV = queueV[carte.snakeV.tail[0]];
        }

        if(carte.fruit<1)
        {
            placer_fruit(&carte);
        }

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
                    for(int k=0; k<carte.snakeV.length; k++)
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

                case TETER:
                    SDL_BlitSurface(teteActuelleR,NULL,ecran,&position);
                    break;

                case CORPR:
                    for(int k=0; k<carte.snakeR.length; k++)
                    {
                        if(carte.snakeR.body[1][k]==i && carte.snakeR.body[2][k]==j)
                        {
                            corpActuelleR=corpR[carte.snakeR.body[0][k]];
                            SDL_BlitSurface(corpActuelleR,NULL,ecran,&position);
                            break;
                        }
                    }
                    break;
                case QUEUER:
                    SDL_BlitSurface(queueActuelleR,NULL,ecran,&position);
                    break;
                }
            }
        }
        sprintf(texteScoreV,"GREEN : %d",carte.snakeV.length);//message);//dijkstratab[1][28][1]);//carte.snakeV.length);//
        scoreV = TTF_RenderText_Blended(police,texteScoreV,couleurOr);
        position.x = 64;
        position.y = 0;
        SDL_BlitSurface(scoreV, NULL, ecran, &position);
        sprintf(texteScoreR,"RED : %d",carte.snakeR.length);
        scoreR = TTF_RenderText_Blended(police,texteScoreR,couleurOr);
        position.x = 800;
        position.y = 0;
        SDL_BlitSurface(scoreR, NULL, ecran, &position);
        SDL_Flip(ecran);
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
    SDL_FreeSurface(scoreV);
    SDL_FreeSurface(fond);
    SDL_FreeSurface(mur);
    SDL_FreeSurface(fruit);
    free(&carte);
}


void deplacerV(Carte *carte)
{
    switch(carte->snakeV.head[0])
    {
    case HAUT:
        if(carte->plateau[carte->snakeV.head[1]-1][carte->snakeV.head[2]]==FRUIT)
        {
            ajouterCorpV(carte);
            carte->snakeV.head[1]--;
            carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]]=TETEV;
            break;
        }
        if(carte->plateau[carte->snakeV.head[1]-1][carte->snakeV.head[2]]!=VIDE)//ou queue a ajouter
        {
            //carte.jouer =0;
            break;
        }
        avancerQueueV(carte);
        avancerCorpV(carte);
        carte->snakeV.head[1]--;
        carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]]=TETEV;
        break;

    case BAS:
        if(carte->plateau[carte->snakeV.head[1]+1][carte->snakeV.head[2]]==FRUIT)
        {
            ajouterCorpV(carte);
            carte->snakeV.head[1]++;
            carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]]=TETEV;
            break;
        }
        if(carte->plateau[carte->snakeV.head[1]+1][carte->snakeV.head[2]]!=VIDE)//ou queue a ajouter
        {
            //carte.jouer =0;
            break;
        }
        avancerQueueV(carte);
        avancerCorpV(carte);
        carte->snakeV.head[1]++;
        carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]]=TETEV;
        break;

    case GAUCHE:
        if(carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]-1]==FRUIT)
        {
            ajouterCorpV(carte);
            carte->snakeV.head[2]--;
            carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]]=TETEV;
            break;
        }
        if(carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]-1]!=VIDE)//ou queue a ajouter
        {
            //carte.jouer =0;
            break;
        }
        avancerQueueV(carte);
        avancerCorpV(carte);
        carte->snakeV.head[2]--;
        carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]]=TETEV;
        break;

    case DROITE:
        if(carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]+1]==FRUIT)
        {
            ajouterCorpV(carte);
            carte->snakeV.head[2]++;
            carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]]=TETEV;
            break;
        }
        if(carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]+1]!=VIDE)//ou queue a ajouter
        {
            //carte.jouer =0;
            break;
        }
        avancerQueueV(carte);
        avancerCorpV(carte);
        carte->snakeV.head[2]++;
        carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]]=TETEV;
        break;
    }
}

void ajouterCorpV(Carte *carte)
{
    carte->fruit--;
    carte->snakeV.length++;
    directionPremierCorpV(carte);
    carte->snakeV.body[1][carte->snakeV.length-1]=carte->snakeV.head[1];
    carte->snakeV.body[2][carte->snakeV.length-1]=carte->snakeV.head[2];
    carte->plateau[carte->snakeV.body[1][carte->snakeV.length-1]][carte->snakeV.body[2][carte->snakeV.length-1 ]]=CORPV;

    Mix_AllocateChannels(32);
    Mix_Chunk *manger = Mix_LoadWAV("manger.wav");
    Mix_PlayChannel(2,manger,0);
}

void avancerCorpV(Carte *carte)
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
            directionPremierCorpV(carte);
        }
        carte->snakeV.body[1][carte->snakeV.length-1]=carte->snakeV.head[1];
        carte->snakeV.body[2][carte->snakeV.length-1]=carte->snakeV.head[2];
        carte->plateau[carte->snakeV.body[1][carte->snakeV.length-1]][carte->snakeV.body[2][carte->snakeV.length-1]]=CORPV;
    }
}

void avancerQueueV(Carte *carte)
{
    carte->snakeV.tail[3]=carte->snakeV.tail[0];
    carte->snakeV.tail[4]=carte->snakeV.tail[1];
    carte->snakeV.tail[5]=carte->snakeV.tail[2];
    carte->plateau[carte->snakeV.tail[1]][carte->snakeV.tail[2]]=VIDE;
    if(carte->snakeV.length>0)
    {
        carte->snakeV.tail[2]=carte->snakeV.body[2][0];
        directionQueueV(carte, carte->snakeV.body[0][0]);
        carte->snakeV.tail[1]=carte->snakeV.body[1][0];
        carte->snakeV.tail[2]=carte->snakeV.body[2][0];

    }
    else
    {
        directionQueueV(carte, carte->snakeV.head[0]);
        carte->snakeV.tail[1]=carte->snakeV.head[1];
        carte->snakeV.tail[2]=carte->snakeV.head[2];
    }
    carte->plateau[carte->snakeV.tail[1]][carte->snakeV.tail[2]]=QUEUEV;
}

void directionPremierCorpV(Carte *carte)
{
    switch(carte->snakeV.head[0])
    {
    case HAUT:
        switch(carte->snakeV.head[3])
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
        }
        break;
    case BAS:
        switch(carte->snakeV.head[3])
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
        }
        break;
    case GAUCHE:
        switch(carte->snakeV.head[3])
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
        }
        break;
    case DROITE:
        switch(carte->snakeV.head[3])
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
        }
        break;
    }
}

void directionQueueV(Carte *carte, int directionPrecedente)
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

void deplacerR(Carte *carte)
{
    switch(carte->snakeR.head[0])
    {
    case HAUT:
        if(carte->plateau[carte->snakeR.head[1]-1][carte->snakeR.head[2]]==FRUIT)
        {
            ajoutercorpR(carte);
            carte->snakeR.head[1]--;
            carte->plateau[carte->snakeR.head[1]][carte->snakeR.head[2]]=TETER;
            break;
        }
        if(carte->plateau[carte->snakeR.head[1]-1][carte->snakeR.head[2]]!=VIDE)//ou queue a ajouter
        {
            //carte.jouer =0;
            break;
        }
        avancerqueueR(carte);
        avancercorpR(carte);
        carte->snakeR.head[1]--;
        carte->plateau[carte->snakeR.head[1]][carte->snakeR.head[2]]=TETER;
        break;

    case BAS:
        if(carte->plateau[carte->snakeR.head[1]+1][carte->snakeR.head[2]]==FRUIT)
        {
            ajoutercorpR(carte);
            carte->snakeR.head[1]++;
            carte->plateau[carte->snakeR.head[1]][carte->snakeR.head[2]]=TETER;
            break;
        }
        if(carte->plateau[carte->snakeR.head[1]+1][carte->snakeR.head[2]]!=VIDE)//ou queue a ajouter
        {
            //carte.jouer =0;
            break;
        }
        avancerqueueR(carte);
        avancercorpR(carte);
        carte->snakeR.head[1]++;
        carte->plateau[carte->snakeR.head[1]][carte->snakeR.head[2]]=TETER;
        break;

    case GAUCHE:
        if(carte->plateau[carte->snakeR.head[1]][carte->snakeR.head[2]-1]==FRUIT)
        {
            ajoutercorpR(carte);
            carte->snakeR.head[2]--;
            carte->plateau[carte->snakeR.head[1]][carte->snakeR.head[2]]=TETER;
            break;
        }
        if(carte->plateau[carte->snakeR.head[1]][carte->snakeR.head[2]-1]!=VIDE)//ou queue a ajouter
        {
            //carte.jouer =0;
            break;
        }
        avancerqueueR(carte);
        avancercorpR(carte);
        carte->snakeR.head[2]--;
        carte->plateau[carte->snakeR.head[1]][carte->snakeR.head[2]]=TETER;
        break;

    case DROITE:
        if(carte->plateau[carte->snakeR.head[1]][carte->snakeR.head[2]+1]==FRUIT)
        {
            ajoutercorpR(carte);
            carte->snakeR.head[2]++;
            carte->plateau[carte->snakeR.head[1]][carte->snakeR.head[2]]=TETER;
            break;
        }
        if(carte->plateau[carte->snakeR.head[1]][carte->snakeR.head[2]+1]!=VIDE)//ou queue a ajouter
        {
            //carte.jouer =0;
            break;
        }
        avancerqueueR(carte);
        avancercorpR(carte);
        carte->snakeR.head[2]++;
        carte->plateau[carte->snakeR.head[1]][carte->snakeR.head[2]]=TETER;
        break;
    }
}

void ajoutercorpR(Carte *carte)
{
    carte->fruit--;
    carte->snakeR.length++;
    directionPremierCorpR(carte);
    carte->snakeR.body[1][carte->snakeR.length-1]=carte->snakeR.head[1];
    carte->snakeR.body[2][carte->snakeR.length-1]=carte->snakeR.head[2];
    carte->plateau[carte->snakeR.body[1][carte->snakeR.length-1]][carte->snakeR.body[2][carte->snakeR.length-1 ]]=CORPR;

    Mix_AllocateChannels(32);
    Mix_Chunk *manger = Mix_LoadWAV("manger.wav");
    Mix_PlayChannel(2,manger,0);
}

void avancercorpR(Carte *carte)
{
    for(int j=0; j<carte->snakeR.length-1; j++)
    {
        carte->snakeR.body[0][j]=carte->snakeR.body[0][j+1];
        carte->snakeR.body[1][j]=carte->snakeR.body[1][j+1];
        carte->snakeR.body[2][j]=carte->snakeR.body[2][j+1];
    }
    if(carte->snakeR.length>0)
    {
        if(carte->snakeR.body[0][carte->snakeR.length-1]!=carte->snakeR.head[0])
        {
            directionPremierCorpR(carte);
        }
        carte->snakeR.body[1][carte->snakeR.length-1]=carte->snakeR.head[1];
        carte->snakeR.body[2][carte->snakeR.length-1]=carte->snakeR.head[2];
        carte->plateau[carte->snakeR.body[1][carte->snakeR.length-1]][carte->snakeR.body[2][carte->snakeR.length-1]]=CORPR;
    }
}

void avancerqueueR(Carte *carte)
{
    carte->snakeR.tail[3]=carte->snakeR.tail[0];
    carte->snakeR.tail[4]=carte->snakeR.tail[1];
    carte->snakeR.tail[5]=carte->snakeR.tail[2];
    carte->plateau[carte->snakeR.tail[1]][carte->snakeR.tail[2]]=VIDE;
    if(carte->snakeR.length>0)
    {
        directionQueueR(carte, carte->snakeR.body[0][0]);
        carte->snakeR.tail[1]=carte->snakeR.body[1][0];
        carte->snakeR.tail[2]=carte->snakeR.body[2][0];

    }
    else
    {
        directionQueueR(carte, carte->snakeR.head[0]);
        carte->snakeR.tail[1]=carte->snakeR.head[1];
        carte->snakeR.tail[2]=carte->snakeR.head[2];
    }
    carte->plateau[carte->snakeR.tail[1]][carte->snakeR.tail[2]]=QUEUER;
}

void directionPremierCorpR(Carte *carte)
{
    switch(carte->snakeR.head[0])
    {
    case HAUT:
        switch(carte->snakeR.head[3])
        {
        case HAUT:
            carte->snakeR.body[0][carte->snakeR.length-1]=HAUT;
            break;
        case GAUCHE:
            carte->snakeR.body[0][carte->snakeR.length-1]=VIRAGEHD;
            break;
        case DROITE:
            carte->snakeR.body[0][carte->snakeR.length-1]=VIRAGEHG;
            break;
        }
        break;
    case BAS:
        switch(carte->snakeR.head[3])
        {
        case BAS:
            carte->snakeR.body[0][carte->snakeR.length-1]=BAS;
            break;
        case GAUCHE:
            carte->snakeR.body[0][carte->snakeR.length-1]=VIRAGEBD;
            break;
        case DROITE:
            carte->snakeR.body[0][carte->snakeR.length-1]=VIRAGEBG;
            break;
        }
        break;
    case GAUCHE:
        switch(carte->snakeR.head[3])
        {
        case HAUT:
            carte->snakeR.body[0][carte->snakeR.length-1]=VIRAGEBG;
            break;
        case BAS:
            carte->snakeR.body[0][carte->snakeR.length-1]=VIRAGEHG;
            break;
        case GAUCHE:
            carte->snakeR.body[0][carte->snakeR.length-1]=GAUCHE;
            break;
        }
        break;
    case DROITE:
        switch(carte->snakeR.head[3])
        {
        case HAUT:
            carte->snakeR.body[0][carte->snakeR.length-1]=VIRAGEBD;
            break;
        case BAS:
            carte->snakeR.body[0][carte->snakeR.length-1]=VIRAGEHD;
            break;
        case DROITE:
            carte->snakeR.body[0][carte->snakeR.length-1]=DROITE;
            break;
        }
        break;
    }
}

void directionQueueR(Carte *carte, int directionPrecedente)
{
    switch(directionPrecedente)
    {
    case HAUT:
        carte->snakeR.tail[0]=HAUT;
        break;
    case BAS:
        carte->snakeR.tail[0]=BAS;
        break;
    case GAUCHE:
        carte->snakeR.tail[0]=GAUCHE;
        break;
    case DROITE:
        carte->snakeR.tail[0]=DROITE;
        break;
    case VIRAGEHG:
        if(carte->snakeR.tail[0]==BAS)
        {
            carte->snakeR.tail[0]=GAUCHE;
        }
        else
        {
            carte->snakeR.tail[0]=HAUT;
        }
        break;
    case VIRAGEHD:
        if(carte->snakeR.tail[0]==BAS)
        {
            carte->snakeR.tail[0]=DROITE;
        }
        else
        {
            carte->snakeR.tail[0]=HAUT;
        }
        break;
    case VIRAGEBG:
        if(carte->snakeR.tail[0]==HAUT)
        {
            carte->snakeR.tail[0]=GAUCHE;
        }
        else
        {
            carte->snakeR.tail[0]=BAS;
        }
        break;
    case VIRAGEBD:
        if(carte->snakeR.tail[0]==HAUT)
        {
            carte->snakeR.tail[0]=DROITE;
        }
        else
        {
            carte->snakeR.tail[0]=BAS;
        }
        break;
    }
}
