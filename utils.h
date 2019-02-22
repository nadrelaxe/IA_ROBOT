//
// Created by alexa on 22/02/2019.
//

#ifndef IA_ROBOT_UTILS_H
#define IA_ROBOT_UTILS_H

#define NB_ZONES            6
#define NB_ROBOT_PAR_ZONE   8
#define NB_ROBOTS           48

typedef struct
{
    int numRobot;
    double vitesse;
}robot;

typedef struct
{
    robot *tabRobot;
    double moyenne;
    double ecartType;
}zone;

double calculerMoyenneZone(zone);

double calculerEcartType(double);

#endif //IA_ROBOT_UTILS_H
