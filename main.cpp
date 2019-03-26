#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <sstream>
#include <time.h>
#include "utils.h"

using namespace std;

#define PATH                    "C:/Users/alexa/Downloads/initial.csv"
#define NB_RUN_PROGRAMME        10000
#define NB_RUN_MELANGE_ZONES    1000

void Init(robot* robotInit, robot* robotModif);
void InitTableauRobots(zone* tabZone, robot* tabRobotModif);
void creationZones(zone* tabZone, robot* tabRobotModif);
void suppressionZones(zone* tabZone);
double runPermutations(zone* tabZone);

void Melanger(robot* tab,int nb);
void EchangeRobotZone(zone* tabZone0, int z1, int r1, int z2, int r2);
void MelangeEntreZone(zone* tabZone0);
void lireCSV(const string &chemin, robot* tab);
int zonePourRobot(zone* tabZone0, int numRobot);
void ecrireResultatCSV(const string &chemin, robot* tabRobot, zone* tabZone0);


int main(int argc, char** argv)
{
    robot tabRobotInit[NB_ROBOTS];
    robot tabRobotModif[NB_ROBOTS];
    zone tabZone[NB_ZONES];
    zone meilleurePermutation[NB_ZONES];
    double ecartType = 0, meilleurEcartType = 1;
    float temps;
    clock_t t1 = clock(), t2;

    srand((unsigned int)time(NULL));

    /*
     * Init des tableaux de robots
     */
    Init(tabRobotInit,tabRobotModif);

    /*
     * Initialisation de la structure pour conserver la meilleur solution
     */
    InitTableauRobots(meilleurePermutation, tabRobotModif);

    for(int index = 0; index < NB_RUN_PROGRAMME; index++)
    {
        /*
         * Creation des zones
         */
        creationZones(tabZone, tabRobotModif);

        /*
        * On effectue 1000 permutations aléatoires entre deux zone choisies aléatoirement
        */
        ecartType = runPermutations(tabZone);

        if(ecartType < meilleurEcartType)
        {
            meilleurEcartType = ecartType;
            cout << index+1 << " / " << NB_RUN_PROGRAMME << " : " << meilleurEcartType << endl;
        }

        //Suppression des zones
        suppressionZones(tabZone);

    }


    //ecrireResultatCSV(PATH, tabRobotInit, tabZone);

    t2 = clock();
    temps = (float)(t2-t1)/CLOCKS_PER_SEC;
    cout << "Temps d'execution : " << temps << " secondes." << endl;

    return 0;
}

void Init(robot* robotInit, robot* robotModif)
{
    lireCSV(PATH, robotInit);
    lireCSV(PATH, robotModif);
}

void InitTableauRobots(zone* tabZone, robot* tabRobotModif)
{
    for(int i = 0; i < NB_ZONES; i++)
    {
        tabZone[i].tabRobot = (robot*)malloc(NB_ROBOT_PAR_ZONE * sizeof(robot));
        for(int j = 0; j < NB_ROBOT_PAR_ZONE; j++)
        {
            tabZone[i].tabRobot[j] = tabRobotModif[(i * NB_ROBOT_PAR_ZONE) + j];
        }
    }
}

void creationZones(zone* tabZone, robot* tabRobotModif)
{
    /*
     * Melange du tableau
     */
    Melanger(tabRobotModif, NB_ROBOTS);

    /*
     * Creation des zones
     */
    InitTableauRobots(tabZone, tabRobotModif);
}

void suppressionZones(zone* tabZone)
{
    /*
     * Suppression des zones
     */
    for(int i=0; i<NB_ZONES; i++)
    {
        free(tabZone[i].tabRobot);
    }
}

double runPermutations(zone *tabZone)
{
    for(int i=0; i<NB_RUN_MELANGE_ZONES; i++)
    {
        MelangeEntreZone(tabZone);
    }

    //Calcul de la moyenne de la répartition et calcul de l'écart type
    double moyenne = calculerMoyenneToutesZones(tabZone);
    return calculerEcartTypeToutesZones(tabZone, moyenne);
}

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