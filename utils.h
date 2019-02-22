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
    char numRobot;
    double moyenne;
    double ecartType;
}zone;

double calculerMoyenneZone(float* tabZone, int nbRobot);

double calculerEcartType(float moyenne);

#endif //IA_ROBOT_UTILS_H
