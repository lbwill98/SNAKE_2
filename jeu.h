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
void deplacerV(Carte *carte);
void ajouterCorpV(Carte *carte);
void avancerCorpV(Carte *carte);
void avancerQueueV(Carte *carte);
void directionPremierCorpV(Carte *carte);
void directionQueueV(Carte *carte, int directionPrecedente);

void deplacerR(Carte *carte);
void ajouterCorpR(Carte *carte);
void avancerCorpR(Carte *carte);
void avancerQueueR(Carte *carte);
void directionPremierCorpR(Carte *carte);
void directionQueueR(Carte *carte, int directionPrecedente);

#endif // JEU_H_INCLUDED
