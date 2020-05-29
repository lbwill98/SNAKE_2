#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <string.h>
#include <SDL/SDL_ttf.h>


int main(int argc, char *argv[])
{
    int speed = 100;
    SDL_Surface *ecran = NULL;
    SDL_Surface **menu = NULL;
    SDL_Surface *texte = NULL;
    SDL_Surface *black = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    SDL_Event eventSettings;
    int continuer = 1;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_putenv("SDL_VIDEO_CENTERED=center");
    SDL_WM_SetIcon(IMG_Load("logo.png"),NULL);
    ecran = SDL_SetVideoMode(960,640,32,SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("SNAKE", NULL);

    menu=(SDL_Surface**)malloc(sizeof(SDL_Surface*)*13);
    menu[0]=IMG_Load("menu0.png");
    menu[1]=IMG_Load("menu1.png");
    menu[2]=IMG_Load("menu2.png");
    menu[3]=IMG_Load("menu3.png");
    menu[4]=IMG_Load("menu4.png");
    menu[5]=IMG_Load("menu5.png");
    menu[6]=IMG_Load("menu6.png");
    menu[7]=IMG_Load("menu7.png");
    menu[8]=IMG_Load("menu8.png");
    menu[9]=IMG_Load("menu9.png");
    menu[10]=IMG_Load("menu10.png");
    menu[11]=IMG_Load("menu11.png");
    menu[12]=IMG_Load("menu12.png");
    black = IMG_Load("plateauNoir2030.png");


    Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS,1024);
    Mix_AllocateChannels(32);
    Mix_Music *musique;
    musique = Mix_LoadMUS("musique.mp3");
    Mix_PlayMusic(musique, -1);

    TTF_Init();
    TTF_Font *police1 = TTF_OpenFont("BKANT.ttf",60);
    TTF_Font *police2 = TTF_OpenFont("BKANT.ttf",30);
    TTF_Font *police3 = TTF_OpenFont("BKANT.ttf",45);
    SDL_Color couleurNoir = {0,0,0}, couleurOr = {255,215,0};

    int i=0;
    while(continuer)
    {
        int continuerSettings = 1;
        SDL_PollEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            SDL_Delay(100);
            continuer = 0;
            break;

        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer=0;
                break;

            case SDLK_1:
                jouer(ecran, speed);
                SDL_Delay(300+speed);
                i=0;
                Mix_PlayMusic(musique, -1);
                break;

            case SDLK_2:
                jouerServeur(ecran, speed);
                SDL_Delay(300+speed);
                i=0;
                Mix_PlayMusic(musique, -1);
                break;

            case SDLK_3:
                jouerClient(ecran, speed);
                SDL_Delay(300+speed);
                i=0;
                Mix_PlayMusic(musique, -1);
                break;

            case SDLK_4:
                positionMenu.x=0;
                positionMenu.y=0;
                SDL_BlitSurface(black, NULL, ecran, &positionMenu);
                texte = TTF_RenderText_Blended(police1,"SPEED",couleurOr);
                positionMenu.x = 390;
                positionMenu.y = 100;
                SDL_BlitSurface(texte, NULL, ecran, &positionMenu);
                texte = TTF_RenderText_Blended(police1,"  SLOW                               FAST     ",couleurOr);
                positionMenu.x = 50;
                positionMenu.y = 200;
                SDL_BlitSurface(texte, NULL, ecran, &positionMenu);
                texte = TTF_RenderText_Blended(police1,"1           2           3           4           5",couleurOr);
                positionMenu.x = 60;
                positionMenu.y = 300;
                SDL_BlitSurface(texte, NULL, ecran, &positionMenu);
                SDL_Flip(ecran);
                while(continuerSettings)
                {
                    SDL_WaitEvent(&eventSettings);
                    switch(eventSettings.type)
                    {
                    case SDL_QUIT:
                        SDL_Delay(300+speed);
                        continuerSettings = 0;
                        break;

                    case SDL_KEYDOWN:
                        switch(eventSettings.key.keysym.sym)
                        {
                        case SDLK_ESCAPE:
                            continuerSettings = 0;
                            break;

                        case SDLK_1:
                            continuerSettings = 0;
                            speed = 160;
                            break;

                        case SDLK_2:
                            continuerSettings = 0;
                            speed = 130;
                            break;

                        case SDLK_3:
                            continuerSettings = 0;
                            speed = 100;
                            break;

                        case SDLK_4:
                            continuerSettings = 0;
                            speed = 70;
                            break;

                        case SDLK_5:
                            continuerSettings = 0;
                            speed = 40;
                            break;
                        }
                        break;
                    }
                }
                SDL_Delay(300+speed);
                i=0;
                break;
            }
            break;
        }
        positionMenu.x=0;
        positionMenu.y=0;
        SDL_BlitSurface(menu[i%13],NULL,ecran,&positionMenu);

        texte = TTF_RenderText_Blended(police1,"SNAKE",couleurOr);
        positionMenu.x = 380;
        positionMenu.y = 10;
        SDL_BlitSurface(texte, NULL, ecran, &positionMenu);

        if(i<26)
        {
            texte = TTF_RenderText_Blended(police3,"press a key",couleurOr);
        }
        else
        {
            texte = TTF_RenderText_Blended(police3,"press a key",couleurNoir);
        }
        positionMenu.x = 375;
        positionMenu.y = 110;
        SDL_BlitSurface(texte, NULL, ecran, &positionMenu);

        texte = TTF_RenderText_Blended(police2,"play vs ia : 1",couleurOr);
        positionMenu.x = 400;
        positionMenu.y = 200;
        SDL_BlitSurface(texte, NULL, ecran, &positionMenu);

        texte = TTF_RenderText_Blended(police2,"play as server : 2",couleurOr);
        positionMenu.x = 370;
        positionMenu.y = 250;
        SDL_BlitSurface(texte, NULL, ecran, &positionMenu);

        texte = TTF_RenderText_Blended(police2,"play as client : 3",couleurOr);
        positionMenu.x = 380;
        positionMenu.y = 300;
        SDL_BlitSurface(texte, NULL, ecran, &positionMenu);

        texte = TTF_RenderText_Blended(police2,"select speed : 4",couleurOr);
        positionMenu.x = 380;
        positionMenu.y = 450;
        SDL_BlitSurface(texte, NULL, ecran, &positionMenu);

        texte = TTF_RenderText_Blended(police2,"select map : 5",couleurOr);
        positionMenu.x = 390;
        positionMenu.y = 500;
        SDL_BlitSurface(texte, NULL, ecran, &positionMenu);

        SDL_Flip(ecran);
        i=(i+1)%52;
    }
    Mix_FreeMusic(musique);
    Mix_CloseAudio();
    SDL_FreeSurface(menu);
    SDL_FreeSurface(texte);
    SDL_FreeSurface(black);
    SDL_QUIT;
    return EXIT_SUCCESS;
}

