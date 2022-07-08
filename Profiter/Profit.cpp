#include "Profit.hpp"

Profit::~Profit() {
    delete holder;
    delete timeEngine;
}

Profit::Profit() {
    holder = new Holder();
    timeEngine = new TimeEngine();
    timeEngine->prepareTimeEngine(holder);
    cout << "start sim" << endl;
    timeEngine->startSimulation(holder);
    cout << "end sim" << endl;
    holder->finalizeSim(&timeEngine->exchanges, &timeEngine->NEAR_EUR, &timeEngine->EUR_NEAR, &timeEngine->nftsSold);
}

