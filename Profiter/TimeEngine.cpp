#include "TimeEngine.hpp"


TimeEngine::~TimeEngine() {
    for (int i = sortedClientsByHour.size()-1; i >= 0; --i) {
        delete sortedClientsByHour.at(i);
    }
}

TimeEngine::TimeEngine() {

}

void TimeEngine::prepareTimeEngine(Holder *holder) {
    config = holder->config;
    bside = holder->bside;
    for (int i = 0; i < 24; ++i) {
        sortedClientsByHour.push_back(new vector<Client*>());
    }
    for (int i = 0; i < holder->clients->size(); ++i) {
        sortedClientsByHour.at(holder->clients->at(i)->dayTimeToCheckMarket)->push_back(holder->clients->at(i));
    }
//    for (int i = 0; i < sortedClientsByHour.size(); ++i) {
//        cout << i << ": " << sortedClientsByHour.at(i)->size() << "\n";
//    }
}

void TimeEngine::startSimulation(Holder* holder) {
    changeEURtoNEAR(holder->days->at(0)->hours.at(0));
//    exchanges.push_back("0-0");

    for (int i = 0; i < config->days; ++i) {
//        cout << "day " + to_string(i) << endl;
        checkMusicianRelease(holder->musicians);

        for (int j = 0; j < sortedClientsByHour.size(); ++j) {
            for (int k = 0; k < sortedClientsByHour.at(j)->size(); ++k) {
                simulateClientActions(sortedClientsByHour.at(j)->at(k), holder->musicians, i, holder->days);
            }
            changeEURtoNEAR(holder->days->at(i)->hours.at(j));
        }
    }
    cout << "-----\n";
    cout << "NFTs sold: " << nftsSold.size() << "\n";
    cout << "NEAR: " << bside->capitalNEAR << "\n";
    cout << "EUR: " << bside->capitalEUR << "\n";
    cout << "-----" << endl;
}

void TimeEngine::simulateClientActions(Client *client, vector<Musician*>* musicians, int day, vector<Day*>* days) {
    for (int i = 0; i < 20; ++i) {
        int index = abs(randomIntReal(0, ((int)musicians->size())));
        Musician* musician = musicians->at(index);
        if (checkBuyMotivation(client, musician)) {
            makeTransaction(client, musician, day, days->at(day)->hours.at(client->dayTimeToCheckMarket));
        }

    }

}

bool TimeEngine::checkBuyMotivation(Client *client, Musician *musician) {
    if (musician->NFTWallet.empty()) {
        return false;
    }

    float price = musician->NFTWallet.at(musician->NFTWallet.size()-1)->value;
    float fullCostForClient = client->kryptoAcceptance ? price : price * (1.0 + config->randomGenClient.changeFee);
    if (client->capital < fullCostForClient) {
        return false;
    }

    float chance = client->motivationToBuy * musician->fame;
    float rnd = randomPercentage();
    if (chance < rnd) {
        return false;
    }
    return true;
}

void TimeEngine::makeTransaction(Client *client, Musician *musician, int day, float stockPrice) {
    NFT* nft = musician->NFTWallet.at(musician->NFTWallet.size()-1);

    float nftPrice = nft->value;
    float clientPrice = nftPrice;
    float musicianPrice = nftPrice * (1.0 - config->bsideCut);
    bside->capitalNEAR += nftPrice * config->bsideCut;

    if (!client->kryptoAcceptance) {
        clientPrice *= (1.0 + config->randomGenClient.changeFee);
        bside->capitalEUR += clientPrice * stockPrice;
        bside->capitalNEAR -= musicianPrice;
        NEAR_EUR.push_back(to_string(day) + "," + to_string(musicianPrice) + "," + to_string(clientPrice*stockPrice));
    }
    if (!musician->kryptoAcceptance) {
        bside->capitalNEAR += musicianPrice;
        musicianPrice *= (1.0 - config->randomGenMusician.changeFee);
        bside->capitalEUR -= musicianPrice * stockPrice ;
        EUR_NEAR.push_back(to_string(day) + "," + to_string(musicianPrice/(1.0 - config->randomGenMusician.changeFee)) + "," + to_string(musicianPrice));
    }

    client->capital -= clientPrice;
    client->creditHistory.push_back(clientPrice*-1.0);
    musician->creditHistory.push_back(musicianPrice);
    client->NFTwallet.push_back(nft);
    musician->NFTWallet.pop_back();

    nft->ownerName = client->name;
    nft->priceAtWhichWasSold.push_back(nftPrice);
    nft->timeAtWhichWasSold.push_back(day);

    string bill = to_string(day) + "," + to_string(nft->id) + "," + nft->bandName + "," + nft->ownerName + "," + to_string(nftPrice) + '\0';
    nftsSold.push_back(bill);
}


void TimeEngine::checkMusicianRelease(vector<Musician*>* musicians) {
    for (int i = 0; i < musicians->size(); ++i) {
        Musician* musician = musicians->at(i);
        if (musician->DaysBetweenReleases.at(0) <= 0) {
            for (int j = 0; j < musician->NFTsPerRelease.at(0); ++j) {
                int nftID = randomIntReal(1000000, 9999999);
                musician->NFTWallet.push_back(new NFT(nftID, musician->PricesPerRelease.at(0), musician->id, musician->name));
            }
            musician->DaysBetweenReleases.erase(musician->DaysBetweenReleases.begin());
            musician->NFTsPerRelease.erase(musician->NFTsPerRelease.begin());
            musician->PricesPerRelease.erase(musician->PricesPerRelease.begin());
        } else {
            musicians->at(i)->DaysBetweenReleases.at(0) -= 1;
        }
    }
}

void TimeEngine::changeEURtoNEAR(float stockPrice) {
    while (bside->capitalNEAR < 100.0) {
        bside->capitalEUR - (500.0 * stockPrice);
        bside->capitalNEAR += 500.0;
        exchanges.push_back(to_string(500.0) + "," + to_string(stockPrice) + "," + to_string(stockPrice*500.0));
    }
}












