#ifndef PROFITER3_TIMEENGINE_HPP
#define PROFITER3_TIMEENGINE_HPP


//#include "Misc/Config.hpp"
#include "Misc/Helper.h"
#include "Holder.hpp"

using namespace std;

class TimeEngine {
public:
    TimeEngine();
    ~TimeEngine();

    void prepareTimeEngine(Holder* holder);
    void startSimulation(Holder* holder);

    void simulateClientActions(Client* client, vector<Musician*>* musicians, int day, vector<Day*>* days);
    bool checkBuyMotivation(Client* client, Musician* musician);
    void makeTransaction(Client* client, Musician* musician, int day, float stockPrice);

    void changeNEAR_EUR();
    void changeEUR_NEAR();

    void checkMusicianRelease(vector<Musician*>* musicians);

    void changeEURtoNEAR(float stockPrice);

    vector<vector<Client*>*> sortedClientsByHour;
    vector<string> exchanges;
    vector<string> NEAR_EUR;
    vector<string> EUR_NEAR;
    vector<string> nftsSold;

private:
    Config* config;
    BSide* bside;



};


#endif //PROFITER3_TIMEENGINE_HPP
