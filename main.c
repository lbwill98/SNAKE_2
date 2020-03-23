#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>


int main(int argc, char *argv[])
{
    SDL_Surface *ecran = NULL;
    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    int continuer = 3;

    SDL_Init(SDL_INIT_VIDEO);

    //SDL_WM_SetIcon(SDL_LoadBMP("link1.bmp"),NULL);
    SDL_WM_SetIcon(IMG_Load("logo.png"),NULL);
    ecran = SDL_SetVideoMode(960,640,32,SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("SNAKE", NULL);
    menu = IMG_Load("menu.png");
    positionMenu.x=0;
    positionMenu.y=0;

    Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS,1024);
    Mix_AllocateChannels(32);
    Mix_Music *musique;
    musique = Mix_LoadMUS("zelda.mp3");
    Mix_PlayMusic(musique, -1);

    while(continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            continuer = 0;
            break;

        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer=0;
                break;

            case SDLK_1:
                //jouer(ecran);
                //Mix_PlayMusic(musique, -1);
                break;
            }
            break;

        }
        SDL_BlitSurface(menu,NULL,ecran,&positionMenu);
        SDL_Flip(ecran);
    }
    Mix_FreeMusic(musique);
    Mix_CloseAudio();
    SDL_FreeSurface(menu);
    SDL_QUIT;
    return EXIT_SUCCESS;

}
