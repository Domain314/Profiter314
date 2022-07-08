#ifndef PROFITER3_HOLDER_HPP
#define PROFITER3_HOLDER_HPP

#include <vector>
#include <iostream>
#include <map>
#include "Misc/Config.hpp"
#include "Misc/Globals.h"
#include "FileOrganizer.hpp"

using namespace std;


class Holder {
public:
    Holder();
    ~Holder();
    void finalizeSim(vector<string>* exchanges, vector<string>* NEAR_EUR, vector<string>* EUR_NEAR, vector<string>* nftsSold);

    Config* config;
    BSide* bside;
    vector<Day*>* days;
    vector<Musician*>* musicians;
    vector<Client*>* clients;

private:
    void initializeHolder();
    void loadUngeneratedFromCSV();

    FileOrganizer* fileOrganizer;




    map<int, Musician*>* musiciansMap;

    map<int, Client*>* clientsMap;

    vector<NFT*>* allNFTs;



};


#endif //PROFITER3_HOLDER_HPP
