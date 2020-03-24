#ifndef JEU_H_INCLUDED
#define JEU_H_INCLUDED

typedef struct
{
    int key[SDLK_LALT]
} Touches;

void updateClavier(Touches *etat_clavier);
void jouer(SDL_Surface* ecran);
void deplacer(int **carte, SDL_Rect *pos, int direction);


#endif // JEU_H_INCLUDED
