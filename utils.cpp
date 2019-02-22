//
// Created by alexa on 22/02/2019.
//

#include "utils.h"
#include <math.h>


double calculerMoyenneZone(zone laZone)
{
    double moyenne = 0;

    for(int i=0; i<NB_ROBOT_PAR_ZONE; i++)
    {
        moyenne += laZone.tabRobot[i].vitesse;
    }

    return  (moyenne / NB_ROBOT_PAR_ZONE);
}


double calculerEcartTypeZone(zone laZone)
{
    double moyenne = laZone.moyenne;
    double variance = 0;

    for(int i=0; i<NB_ROBOT_PAR_ZONE; i++)
    {
        variance += pow(laZone.tabRobot[i].vitesse - moyenne,2);
    }

    return sqrt(variance / NB_ROBOT_PAR_ZONE);
}

double calculerMoyenneToutesZones(zone* tabZones)
{
    double moyenneZones = 0;

    for(int i=0; i<NB_ZONES; i++)
    {
        tabZones[i].moyenne = calculerMoyenneZone(tabZones[i]);
        moyenneZones += tabZones[i].moyenne;
    }

    return moyenneZones / NB_ZONES;
}

double calculerEcartTypeToutesZones(zone* tabZones, double moyenneZones)
{
    double variance = 0;

    for(int i=0; i<NB_ZONES; i++)
    {
        tabZones[i].ecartType = calculerEcartTypeZone(tabZones[i]);
        variance += pow(tabZones[i].moyenne - moyenneZones, 2);
    }

    return sqrt(variance/NB_ZONES);
}

