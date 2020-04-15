#include <stdlib.h>
#include <stdio.h>

#include "ia.h"

void aiV1(Carte *carte)
{
    int fruit[2];
    for(int i=0; i<carte->lignes; i++)
    {
        for(int j=0; j<carte->colonnes; j++)
        {
            if(carte->plateau[i][j]==FRUIT)
            {
                fruit[0]=i;
                fruit[1]=j;
            }
        }
    }
    if(carte->snakeV.head[1]>fruit[0] && (carte->plateau[carte->snakeV.head[1]-1][carte->snakeV.head[2]] ==VIDE || carte->plateau[carte->snakeV.head[1]-1][carte->snakeV.head[2]] ==FRUIT))
    {
        carte->snakeV.head[0]=HAUT;
    }
    if(carte->snakeV.head[1]<fruit[0] && (carte->plateau[carte->snakeV.head[1]+1][carte->snakeV.head[2]] ==VIDE || carte->plateau[carte->snakeV.head[1]+1][carte->snakeV.head[2]] ==FRUIT))
    {
        carte->snakeV.head[0]=BAS;
    }
    if(carte->snakeV.head[2]>fruit[1] && (carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]-1] ==VIDE || carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]-1] ==FRUIT))
    {
        carte->snakeV.head[0]=GAUCHE;
    }
    if(carte->snakeV.head[2]<fruit[1] && (carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]+1] ==VIDE || carte->plateau[carte->snakeV.head[1]][carte->snakeV.head[2]+1] ==FRUIT))
    {
        carte->snakeV.head[0]=DROITE;
    }
}

void aiR1(Carte *carte)
{
    int fruit[2];
    for(int i=0; i<carte->lignes; i++)
    {
        for(int j=0; j<carte->colonnes; j++)
        {
            if(carte->plateau[i][j]==FRUIT)
            {
                fruit[0]=i;
                fruit[1]=j;
            }
        }
    }
    if(carte->snakeR.head[1]>fruit[0] && (carte->plateau[carte->snakeR.head[1]-1][carte->snakeR.head[2]] ==VIDE || carte->plateau[carte->snakeR.head[1]-1][carte->snakeR.head[2]] ==FRUIT))
    {
        carte->snakeR.head[0]=HAUT;
    }
    if(carte->snakeR.head[1]<fruit[0] && (carte->plateau[carte->snakeR.head[1]+1][carte->snakeR.head[2]] ==VIDE || carte->plateau[carte->snakeR.head[1]+1][carte->snakeR.head[2]] ==FRUIT))
    {
        carte->snakeR.head[0]=BAS;
    }
    if(carte->snakeR.head[2]>fruit[1] && (carte->plateau[carte->snakeR.head[1]][carte->snakeR.head[2]-1] ==VIDE || carte->plateau[carte->snakeR.head[1]][carte->snakeR.head[2]-1] ==FRUIT))
    {
        carte->snakeR.head[0]=GAUCHE;
    }
    if(carte->snakeR.head[2]<fruit[1] && (carte->plateau[carte->snakeR.head[1]][carte->snakeR.head[2]+1] ==VIDE || carte->plateau[carte->snakeR.head[1]][carte->snakeR.head[2]+1] ==FRUIT))
    {
        carte->snakeR.head[0]=DROITE;
    }
}

void aiR2(Carte *carte)//c'est dijkstra dans le cas ou chaque arc vaut 1. on le fait en dynamique, cad une fois pas coup car le plateau change
{
    int ***adjtab;//matrice d'adjacence, pour chaque case, dit si in peut aller, en HAUT,BAS,GAUCHE ou DROITE dans la troisiemme coordonnnée
    adjtab = (int ***)malloc(carte->lignes*sizeof(int **));
    for (int i=0; i < carte->lignes; i++)
    {
        adjtab[i] = (int **)malloc(carte->colonnes*sizeof(int*));
        for (int j=0; j < carte->colonnes; j++)
        {
            adjtab[i][j] = (int *)malloc(4*sizeof(int));
        }
    }
    initadjtabR(carte,adjtab);

    int ***dijkstratab;
    //tableau pour l'algo de dijkstra, [0]la distance, [1]la direction precedente
    dijkstratab = (int ***)malloc(carte->lignes*sizeof(int **));
    for (int i=0; i < carte->lignes; i++)
    {
        dijkstratab[i] = (int **)malloc(carte->colonnes*sizeof(int*));
        for (int j=0; j < carte->colonnes; j++)
        {
            dijkstratab[i][j] = (int *)malloc(2*sizeof(int));
        }
    }
    initdijktsatabR(carte,dijkstratab);
    //on construit le tableau de l'algo de dijkstra et on s'arrète quant on trouve un fruit
    int fruit[2]={-1,-1};
    int cherche = 1;
    int distanceActuelle = 0;
    while(cherche)
    {
        for (int i=0; i < carte->lignes; i++)
        {
            for (int j=0; j < carte->colonnes; j++)
            {
                if(dijkstratab[i][j][0]==distanceActuelle)
                {
                    if(carte->plateau[i][j]==FRUIT)
                    {
                        cherche = 0;
                        fruit[0]=i;
                        fruit[1]=j;
                    }
                    if(dijkstratab[i-1][j][0]==600 && adjtab[i][j][HAUT])
                    {
                        dijkstratab[i-1][j][0]=dijkstratab[i][j][0]+1;
                        dijkstratab[i-1][j][1]=HAUT;
                    }
                    if(dijkstratab[i+1][j][0]==600 && adjtab[i][j][BAS])
                    {
                        dijkstratab[i+1][j][0]=dijkstratab[i][j][0]+1;
                        dijkstratab[i+1][j][1]=BAS;
                    }
                    if(dijkstratab[i][j-1][0]==600 && adjtab[i][j][GAUCHE])
                    {
                        dijkstratab[i][j-1][0]=dijkstratab[i][j][0]+1;
                        dijkstratab[i][j-1][1]=GAUCHE;
                    }
                    if(dijkstratab[i][j+1][0]==600 && adjtab[i][j][DROITE])
                    {
                        dijkstratab[i][j+1][0]=dijkstratab[i][j][0]+1;
                        dijkstratab[i][j+1][1]=DROITE;
                    }
                }
            }
        }
        distanceActuelle++;
        /*if(distanceActuelle==50)
        {
            cherche =0;
        }*/
    }
    //on remonte les directions du fruit jusqu'à la tete
    //int distanceActuelle = dijkstratab[fruit[0]][fruit[1]][0];//on garde fruit pour remonter
    int directionPrecedente;
    while (distanceActuelle>0)
    {
        directionPrecedente = dijkstratab[fruit[0]][fruit[1]][1];
        if(directionPrecedente==HAUT)
        {
            fruit[0]++;
        }
        if(directionPrecedente==BAS)
        {
            fruit[0]--;
        }
        if(directionPrecedente==GAUCHE)
        {
            fruit[1]++;
        }
        if(directionPrecedente==DROITE)
        {
            fruit[1]--;
        }
        distanceActuelle = dijkstratab[fruit[0]][fruit[1]][0];
    }
    //on affecte la première direction à prendre à la tete
    carte->snakeR.head[0]=directionPrecedente;
}

void initadjtabR(Carte *carte, int*** adjtab)
{
    for(int i=0; i<carte->lignes;i++)
    {
        for(int j=0; j<carte->colonnes;j++)
        {
            if(carte->plateau[i][j]==VIDE || carte->plateau[i][j]==FRUIT || carte->plateau[i][j]==TETER)
            {
                if(carte->plateau[i-1][j]==VIDE || carte->plateau[i-1][j]==FRUIT)
                {
                    adjtab[i][j][HAUT]=1;
                }
                else
                {
                    adjtab[i][j][HAUT]=0;
                }
                if(carte->plateau[i+1][j]==VIDE || carte->plateau[i+1][j]==FRUIT)
                {
                    adjtab[i][j][BAS]=1;
                }
                else
                {
                    adjtab[i][j][BAS]=0;
                }
                if(carte->plateau[i][j-1]==VIDE || carte->plateau[i][j-1]==FRUIT)
                {
                    adjtab[i][j][GAUCHE]=1;
                }
                else
                {
                    adjtab[i][j][GAUCHE]=0;
                }
                if(carte->plateau[i][j+1]==VIDE || carte->plateau[i][j+1]==FRUIT)
                {
                    adjtab[i][j][DROITE]=1;
                }
                else
                {
                    adjtab[i][j][DROITE]=0;
                }
            }
            else
            {
                adjtab[i][j][HAUT]=0;
                adjtab[i][j][BAS]=0;
                adjtab[i][j][GAUCHE]=0;
                adjtab[i][j][DROITE]=0;
            }
        }
    }
}

void initdijktsatabR(Carte *carte,int*** dijkstratab)
{
    for(int i=0; i<carte->lignes;i++)
    {
        for(int j=0; j<carte->colonnes;j++)
        {
            dijkstratab[i][j][1]=-1;
            if(carte->plateau[i][j]==TETER)
            {
                dijkstratab[i][j][0]=0;
            }
            else
            {
                dijkstratab[i][j][0]=600;//cad +INF
            }
        }
    }
}
