#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <sstream>
#include "utils.h"

using namespace std;

#define PATH "C:\Users\Coren\Downloads\initial.csv"

//double vitesses[48] = {1.28156314102564,1.26833525641026,1.29170064102564,1.25450416666667,1.42281378205128,1.39077435897436,1.37508525641026,1.10262820512821,1.27651858974359,1.21970865384615,1.38303525641026,1.11068301282051,1.52128878205128,1.39518173076923,1.25415256410256,1.49925288461538,1.26601826923077,1.3676108974359,1.242825,1.27468717948718,1.18235512820513,1.09703846153846,1.4667358974359,1.41578974358974,1.19993108974359,1.23851538461538,1.38222788461538,1.13083717948718,1.2643782051282,1.25529871794872,1.14160769230769,1.34871121794872,1.36339006410256,1.12933429487179,1.21889967948718,1.3537125,1.41010256410256,1.17621570512821,1.38086121794872,1.32788621794872,1.50639455128205,1.11700865384615,1.42255384615385,1.37143846153846,1.43075993589744,1.56781025641026,1.17099487179487,1.3703233974359};

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

void EchangeRobotZone(zone* tabZone0, int z1, int r1, int z2, int r2)
{
    int numRobotTmp = tabZone0[z1].tabRobot[r1].numRobot;
    double vitesseTmp = tabZone0[z1].tabRobot[r1].vitesse;

    tabZone0[z1].tabRobot[r1].numRobot = tabZone0[z2].tabRobot[r2].numRobot;
    tabZone0[z1].tabRobot[r1].vitesse = tabZone0[z2].tabRobot[r2].vitesse;

    tabZone0[z2].tabRobot[r2].numRobot = numRobotTmp;
    tabZone0[z2].tabRobot[r2].vitesse = vitesseTmp;
}

void MelangeEntreZone(zone* tabZone0)
{
    int z1 = rand()%NB_ZONES;
    int z2 = rand()%NB_ZONES;
    while (z1 == z2)
    {
        z2 = rand()%NB_ZONES;
    }

    int i, j;

    double moyenne = 0;
    double ecartType1 = 0;
    double ecartType2 = 0;

    for(i = 0;i < NB_ROBOT_PAR_ZONE;i++)
    {
        for(j = 0;j < NB_ROBOT_PAR_ZONE;j++)
        {
            moyenne = calculerMoyenneToutesZones(tabZone0);
            ecartType1 = calculerEcartTypeToutesZones(tabZone0, moyenne);

            EchangeRobotZone(tabZone0, z1, i, z2, j);

            moyenne = calculerMoyenneToutesZones(tabZone0);
            ecartType2 = calculerEcartTypeToutesZones(tabZone0, moyenne);

            if (ecartType1 < ecartType2)
            {
                EchangeRobotZone(tabZone0, z1, i, z2, j);
            }
        }
    }
}

void lireCSV(const string &chemin, robot* tab)
{
    ifstream csvFile;
    csvFile.open(chemin.c_str());

    if (!csvFile.is_open())
    {
        cout << "Path Wrong!!!!" << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    vector <string> vec;
    getline(csvFile, line); // skip the 1st line

    int i = 0;
    while (getline(csvFile,line))
    {
        if (line.empty()) // skip empty lines:
        {
            //cout << "empty line!" << endl;
            continue;
        }

        istringstream iss(line);
        string lineStream;
        string::size_type sz;

        vector <long double> row;

        while (getline(iss, lineStream, ','))
        {
            row.push_back(stold(lineStream,&sz)); // convert to double
        }

        tab[i].numRobot = row[0];
        tab[i].vitesse = row[1];
        i++;

    }
    csvFile.close();
}

int zonePourRobot(zone* tabZone0, int numRobot)
{
    bool stopBoucle = false;
    int i;
    for (i = 0; (i < NB_ZONES) && !stopBoucle; i++)
    {
        for (int j = 0; (j < NB_ROBOT_PAR_ZONE) && !stopBoucle; j++)
        {
            if (tabZone0[i].tabRobot[j].numRobot == numRobot)
            {
                stopBoucle = true;
            }
        }
    }

    return (i - 1);
}

void ecrireResultatCSV(const string &chemin, robot* tabRobot, zone* tabZone0)
{
    string tabS [100];

    ifstream csvFile;
    csvFile.open(chemin.c_str());

    if (!csvFile.is_open())
    {
        cout << "Path Wrong!!!!" << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    vector <string> vec;
    getline(csvFile, line); // skip the 1st line

    tabS[0] = line;

    int i = 0;
    while (getline(csvFile,line))
    {
        if (line.empty()) // skip empty lines:
        {
            //cout << "empty line!" << endl;
            continue;
        }

        tabS[++i] = line + std::to_string(zonePourRobot(tabZone0, tabRobot[i].numRobot));
    }

    csvFile.close();

    std::ofstream myfile;
    myfile.open(chemin.c_str(), std::ofstream::out | std::ofstream::trunc);

    for (i = 0; i < NB_ROBOTS + 1; i++)
    {
        myfile << tabS[i] << "\n";
    }

    myfile.close();
}


int main(int argc, char** argv)
{
    robot tabRobotInit[NB_ROBOTS];
    robot tabRobotModif[NB_ROBOTS];
    zone tabZone[NB_ZONES];

    srand((unsigned int)time(NULL));

    /*
     * Init du tableau de robots
     */
    lireCSV("C:/Users/Coren/Downloads/initial.csv", tabRobotInit);
    lireCSV("C:/Users/Coren/Downloads/initial.csv", tabRobotModif);
    /*for(int i=0; i<NB_ROBOTS; i++)
    {
        tabRobot[i].numRobot = i + 1;
        tabRobot[i].vitesse = vitesses[i];
    }*/

    /*
     * Melange du tableau
     */
    Melanger(tabRobotModif, NB_ROBOTS);

    /*
     * Cration des zones
     */
    for(int i=0; i<NB_ZONES; i++)
    {
        tabZone[i].tabRobot = (robot*)malloc(NB_ROBOT_PAR_ZONE * sizeof(robot));
        for(int j = 0; j < NB_ROBOT_PAR_ZONE; j++)
        {
            tabZone[i].tabRobot[j] = tabRobotModif[(i * NB_ROBOT_PAR_ZONE) + j];
        }
    }

    for(int i=0; i<1000; i++)
    {
        MelangeEntreZone(tabZone);
    }

    //Calcul de la moyenne de la répartition et calcul de l'écart type
    double moyenne = calculerMoyenneToutesZones(tabZone);
    double ecartType = calculerEcartTypeToutesZones(tabZone, moyenne);

    //Affichage
    cout << ecartType << endl;

    ecrireResultatCSV("C:/Users/Coren/Downloads/initial.csv", tabRobotInit, tabZone);

    return 0;
}
