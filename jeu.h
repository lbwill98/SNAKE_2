#ifndef JEU_H_INCLUDED
#define JEU_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <windows.h>
#include <conio.h>
#include "utilitaires.h"



void updateClavier(Touches *etat_clavier);
void jouer(SDL_Surface* ecran);
void deplacer(Carte *carte);
void ajouterCorp(Carte *carte);
void avancerCorp(Carte *carte);
void avancerQueue(Carte *carte);
void directionPremierCorp(Carte *carte, int directionPrecedente);
void directionQueue(Carte *carte, int directionPrecedente);

#endif // JEU_H_INCLUDED
