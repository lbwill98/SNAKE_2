#ifndef JEU_H_INCLUDED
#define JEU_H_INCLUDED

#include "utilitaires.h"



void updateClavier(Touches *etat_clavier);
void jouer(SDL_Surface* ecran);
void deplacer(Carte *carte);

#endif // JEU_H_INCLUDED
