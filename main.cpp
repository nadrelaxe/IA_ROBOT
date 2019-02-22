#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include "utils.h"

using namespace std;

double vitesses[48] = {1.28156314102564,1.26833525641026,1.29170064102564,1.25450416666667,1.42281378205128,1.39077435897436,1.37508525641026,1.10262820512821,1.27651858974359,1.21970865384615,1.38303525641026,1.11068301282051,1.52128878205128,1.39518173076923,1.25415256410256,1.49925288461538,1.26601826923077,1.3676108974359,1.242825,1.27468717948718,1.18235512820513,1.09703846153846,1.4667358974359,1.41578974358974,1.19993108974359,1.23851538461538,1.38222788461538,1.13083717948718,1.2643782051282,1.25529871794872,1.14160769230769,1.34871121794872,1.36339006410256,1.12933429487179,1.21889967948718,1.3537125,1.41010256410256,1.17621570512821,1.38086121794872,1.32788621794872,1.50639455128205,1.11700865384615,1.42255384615385,1.37143846153846,1.43075993589744,1.56781025641026,1.17099487179487,1.3703233974359};

void Melanger(robot* tab,int nb)
{
    int i,nb2;
    nb2 = nb;
    for(i=0;i<nb;i++)
    {
        robot tmp;
        int index = rand()%nb2;
        tmp = tab[index];
        tab[index] = tab[nb2-1];
        tab[nb-i-1] = tmp;
        nb2--;
    }
}

int main(int argc, char** argv)
{
    robot tabRobot[NB_ROBOTS];

    for(int i=0; i<NB_ROBOTS; i++)
    {
        tabRobot[i].numRobot = i + 1;
        tabRobot[i].vitesse = vitesses[i];
    }

    srand(time(NULL));
    Melanger(tabRobot, NB_ROBOTS);

    zone tabZone[NB_ZONES];
    for(int i=0; i<NB_ZONES; i++)
    {
        tabZone[i].tabRobot = (robot*)malloc(NB_ROBOT_PAR_ZONE * sizeof(robot));
        for(int j = 0; j < NB_ROBOT_PAR_ZONE; j++)
        {
            tabZone[i].tabRobot[j] = tabRobot[(i * NB_ROBOT_PAR_ZONE) + j];
        }
    }
}