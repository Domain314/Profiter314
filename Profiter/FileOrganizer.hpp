#ifndef PROFITER3_FILEORGANIZER_HPP
#define PROFITER3_FILEORGANIZER_HPP

#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Misc/Config.hpp"
#include "Misc/Helper.h"

using namespace std;


class FileOrganizer {
public:
    FileOrganizer();
    ~FileOrganizer();

    Config* createConfig(BSide* bside);

    vector<Day*>* randomStocksFromConfig();
    vector<Musician*>* randomMusiciansFromConfig();
    vector<Client*>* randomClientsFromConfig();

    void saveStocksAsCSV(string name, vector<Day*>* days);
    void saveMusiciansAsCSV(string name, vector<Musician*>* musicians);
    void saveClientsAsCSV(string name, vector<Client*>* clients);

    int readAsMap(map<string, string> *configMap);

    vector<Day*>* loadStocksFromCSV();
    vector<Musician*>* loadMusiciansFromCSV();
    vector<Client*>* loadClientsFromCSV();

    vector<string> readFromTXTFile(string path, bool config);
    vector<string> readFromCSVFile(string path, int columns);

    void saveSimToCSV(vector<string>* exchanges, vector<string>* NEAR_EUR, vector<string>* EUR_NEAR, vector<string>* nftsSold);
    void writeVectorStringToCSV(string name, vector<string>* vecStr, string headline);
    void writeBsideResult(float NEAR, float EUR);

    Config* config;
};


#endif //PROFITER3_FILEORGANIZER_HPP
