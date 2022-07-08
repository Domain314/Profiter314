#include "Holder.hpp"

Holder::~Holder() {
    for (int i = days->size()-1; i >= 0; --i) {
        delete days->at(i);
    }
    delete days;
    for (int i = musicians->size()-1; i >= 0; --i) {
        for (int j = musicians->at(i)->NFTWallet.size()-1; j >= 0; --j) {
            delete musicians->at(i)->NFTWallet.at(j);
        }
        delete musicians->at(i);
    }
    delete musicians;
    for (int i = clients->size()-1; i >= 0; --i) {
        for (int j = clients->at(i)->NFTwallet.size()-1; j >= 0; --j) {
            delete clients->at(i)->NFTwallet.at(j);
        }
        delete clients->at(i);
    }
    delete clients;
    delete bside;
    delete config;
    delete fileOrganizer;
}

Holder::Holder() {
    fileOrganizer = new FileOrganizer();
    bside = new BSide();
    config = fileOrganizer->createConfig(bside);
    initializeHolder();
}

void Holder::initializeHolder() {
    if (config->mode[0] == 'g') {
        days = fileOrganizer->randomStocksFromConfig();
    }
    if (config->mode[1] == 'g') {
        musicians = fileOrganizer->randomMusiciansFromConfig();
    }
    if (config->mode[2] == 'g') {
        clients = fileOrganizer->randomClientsFromConfig();
    }
    loadUngeneratedFromCSV();
}

void Holder::loadUngeneratedFromCSV() {
    if (days == nullptr) {
        cout << "Stock empty. Load From CSV: " << config->pathToLoadStock << "\n";
        days = fileOrganizer->loadStocksFromCSV();
        vector<string> strArray = fileOrganizer->readFromTXTFile(config->pathToLoadStock, false);
        cout << "loaded";
    }
    if (musicians == nullptr) {
        cout << "Musicians empty. Load From CSV: " << config->pathToLoadMusicians << "\n";
        musicians = fileOrganizer->loadMusiciansFromCSV();
        vector<string> strArray = fileOrganizer->readFromTXTFile(config->pathToLoadMusicians, false);
        cout << "loaded";
    }
    if (clients == nullptr) {
        cout << "Clients empty. Load From CSV: " << config->pathToLoadClients << "\n";
        clients = fileOrganizer->loadClientsFromCSV();
        vector<string> strArray = fileOrganizer->readFromTXTFile(config->pathToLoadClients, false);
        cout << "loaded";
    }
}

void Holder::finalizeSim(vector<string>* exchanges, vector<string>* NEAR_EUR, vector<string>* EUR_NEAR, vector<string>* nftsSold) {
    fileOrganizer->saveSimToCSV(exchanges, NEAR_EUR, EUR_NEAR, nftsSold);
    fileOrganizer->saveMusiciansAsCSV("RESULT_SIM/Musicians", musicians);
    fileOrganizer->saveClientsAsCSV("RESULT_SIM/Clients", clients);
    fileOrganizer->writeBsideResult(bside->capitalNEAR, bside->capitalEUR);

}
