#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <pthread.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

#include <winsock2.h>
#include <string.h>
#pragma comment(lib, "ws2_32.lib")

#define LEPORT 2020

#include "jeu.h"

int messageClient = -1;
void jouerClient(SDL_Surface* ecran, int speed)
{
    WSADATA WSAData;
    int taille,bd,lg=10;
    SOCKET sock;
    SOCKADDR_IN  sin; //adresse internet locale
    SOCKADDR_IN csin; //pointeur vers adresse internet expediteur (recuperée de l'entete paquet UDP recu)
    char *name = "localhost";
    char *adr_serv="127.0.0.1";//"192.168.1.16";

    struct hostent *host;

    /* RAPPEL de struct hostent, definie dans netdb.h
    struct  hostent {
    	char    *h_name;        // official name of host
    	char    **h_aliases;    // alias list
    	int     h_addrtype;     // host address type
    	int     h_length;       // length of address
    	char    **h_addr_list;  // list of addresses from name server
    };
    */
    WSAStartup(MAKEWORD(2,0), &WSAData);
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    printf("le socket est identifie par : %d \n",sock);

    taille = sizeof(sin);

// Avec le DNS, on obtient l'adresse ip par gethostbyname()
    if ((host = gethostbyname(name))==NULL)
    {
        perror("Nom de machine");
        exit(2);
    };
    memcpy (host -> h_addr_list[0], &sin.sin_addr,host -> h_length);

// si on connait l'adresse ip de destination en chiffre, on peut l'affecter directement:
    //un_entier=inet_aton(adr_serv, &padin.sin_addr);
    sin.sin_addr.s_addr = inet_addr(adr_serv);
    sin.sin_family = AF_INET;
    sin.sin_port   = htons(LEPORT);


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





    int sinsize = sizeof(csin);
    bd= recvfrom(sock, &carte.speed, lg, 0, (SOCKADDR *)&csin, &sinsize);
    while(carte.jouer==1)
    {
        carte.snakeV.head[3]=carte.snakeV.head[0];
        carte.snakeR.head[3]=carte.snakeR.head[0];

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

        bd = sendto(sock,&carte.snakeR.head[0],lg,0,&sin,taille);

        sinsize = sizeof(csin);
        bd= recvfrom(sock, &carte.snakeV.head[0], lg, 0, (SOCKADDR *)&csin, &sinsize);

        sinsize = sizeof(csin);
        bd= recvfrom(sock, &carte.positionFruit[0], lg, 0, (SOCKADDR *)&csin, &sinsize);

        sinsize = sizeof(csin);
        bd= recvfrom(sock, &carte.positionFruit[1], lg, 0, (SOCKADDR *)&csin, &sinsize);

        carte.plateau[carte.positionFruit[0]][carte.positionFruit[1]]=FRUIT;
        carte.fruit=1;

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
        sprintf(texteScoreV,"VERT : %d",carte.snakeV.length);//messageClient);//dijkstratab[1][28][1]);//
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

    closesocket(sock);
    WSACleanup();
}



