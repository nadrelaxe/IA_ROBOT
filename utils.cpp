//
// Created by alexa on 22/02/2019.
//

#include "utils.h"

double calculerMoyenneZone(zone laZone)
{
    double moyenne;

    for(int i=0; i<NB_ROBOT_PAR_ZONE; i++)
    {
        moyenne += laZone.tabRobot[i];
    }

    return  (moyenne/NB_ROBOT_PAR_ZONE);
}

double calculerEcartType(double moyenne)
{

}