#ifndef IA_H_INCLUDED
#define IA_H_INCLUDED

#include "utilitaires.h"

void aiV1(Carte *carte);
void aiR1(Carte *carte);
void aiR2(Carte *carte);
void initadjtabR(Carte *carte,int ***adjtab);
void initdijktsatabR(Carte *carte, int*** dijkstratab);

#endif // IA_H_INCLUDED
