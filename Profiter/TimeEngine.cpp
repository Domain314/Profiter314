#include "TimeEngine.hpp"

// Destructor
TimeEngine::~TimeEngine() {
    for (int i = sortedClientsByHour.size()-1; i >= 0; --i) {
        delete sortedClientsByHour.at(i);
    }
}

// Constructor
TimeEngine::TimeEngine() { }

// Save copy of config and bside pointer, to have more readable code
// First loop: fill the 2D vector<vector<Client*>> with 24 empty vector<Client*>
// Second loop: add Client* to 2D vector at index = client's time to check market.
// (client's time = 13:00   =>   client saved at index [13])
void TimeEngine::prepareTimeEngine(Holder *holder) {
    config = holder->config;
    bside = holder->bside;
    for (int i = 0; i < 24; ++i) {
        sortedClientsByHour.push_back(new vector<Client*>());
    }
    for (int i = 0; i < holder->clients->size(); ++i) {
        sortedClientsByHour.at(holder->clients->at(i)->dayTimeToCheckMarket)->push_back(holder->clients->at(i));
    }
}

// First, change EUR to some NEAR, if needed.
// Parent Loop: For each day (from config) check for musicians who make an NFT release.
// Child Loop: For all hours (24 entries in 2d vector) do GrandChildLoop and buy new NEAR if needed.
// Grand Child Loop: For all clients who check the market in this hour, check their action.
void TimeEngine::startSimulation(Holder* holder) {
    changeEURtoNEAR(holder->days->at(0)->hours.at(0), 0);

    for (int i = 0; i < config->days; ++i) {
        checkMusicianRelease(holder->musicians);

        for (int j = 0; j < sortedClientsByHour.size(); ++j) {
            for (int k = 0; k < sortedClientsByHour.at(j)->size(); ++k) {
                simulateClientActions(sortedClientsByHour.at(j)->at(k), holder->musicians, i, holder->days);
            }
            changeEURtoNEAR(holder->days->at(i)->hours.at(j), i);
        }
    }
    cout << "-----\n";
    cout << "NFTs sold: " << nftsSold.size() << "\n";
    cout << "NEAR: " << bside->capitalNEAR << "\n";
    cout << "EUR: " << bside->capitalEUR << "\n";
    cout << "-----" << endl;
}

// Client checks x random Musicians (based on his amountMusiciansPerSession)
// If the Client passes his motivation check, he buys the last released NFT of this musician
void TimeEngine::simulateClientActions(Client *client, vector<Musician*>* musicians, int day, vector<Day*>* days) {
    for (int i = 0; i < client->amountMusiciansPerSession; ++i) {
        int index = abs(randomIntReal(0, ((int)musicians->size())));
        Musician* musician = musicians->at(index);
        if (checkBuyMotivation(client, musician)) {
            makeTransaction(client, musician, day, days->at(day)->hours.at(client->dayTimeToCheckMarket));
        }

    }

}

// To pass the motivation check:
// - the musicians NFT-Wallet can't be empty
// - client needs enough capital
// - client's motivation score, multiplied by the popularity score of the musician,
//   has to be higher, than a random Percentage (equal distribution)
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

// 1. Get NFT and its prices for every side
// 2. calculate new costs, if krypto acceptance is "false"
// 3. subtract from clients capital, add info to histories and transfer NFT from one wallet to other
// 4. change NFT owner and add to its history, the price and day
// 5. make a bill-string and add it to history
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

        string bill =
                to_string(day) + "," +
                to_string(nftPrice) + "," +
                to_string(nftPrice * config->randomGenClient.changeFee) + "," +
                to_string(clientPrice) + "," +
                to_string(clientPrice * stockPrice);
        EUR_NEAR.push_back(bill);
    }
    if (!musician->kryptoAcceptance) {
        string bill =
                to_string(day) + "," +
                to_string(musicianPrice) + "," +
                to_string(musicianPrice * config->randomGenMusician.changeFee) + ",";

        bside->capitalNEAR += musicianPrice;
        musicianPrice *= (1.0 - config->randomGenMusician.changeFee);
        bside->capitalEUR -= musicianPrice * stockPrice;

        bill += to_string(musicianPrice) + "," +
                to_string(musicianPrice * stockPrice);
        NEAR_EUR.push_back(bill);
    }

    client->capital -= clientPrice;
    client->creditHistory.push_back(clientPrice*-1.0);
    musician->creditHistory.push_back(musicianPrice);
    client->NFTwallet.push_back(nft);
    musician->NFTWallet.pop_back();

    nft->ownerName = client->name;
    nft->priceAtWhichWasSold.push_back(nftPrice);
    nft->timeAtWhichWasSold.push_back(day);

    string bill =
            to_string(day) + "," +
            to_string(nft->id) + "," +
            nft->bandName + "," +
            to_string(musician->kryptoAcceptance) + "," +
            nft->ownerName + "," +
            to_string(client->kryptoAcceptance) + "," +
            to_string(nftPrice) + "," +
            to_string(clientPrice) + "," +
            to_string(musicianPrice) + "," +
            to_string(clientPrice - musicianPrice) + "," + '\0';
    nftsSold.push_back(bill);
}

// Check which musicians time has come to release new NFTs
// If it's time, add the amount of NFTs to the NFT-Wallet and for the price, randomly chosen, while generating.
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

// Buy NEAR, when NEAR capital is under a 5th of the maxExchangePerHour
void TimeEngine::changeEURtoNEAR(float stockPrice, int day) {
    while (bside->capitalNEAR < bside->exchangeAmount/5) {
        float near = bside->exchangeAmount;
        bside->capitalEUR - (near * stockPrice);
        bside->capitalNEAR += near;
        exchanges.push_back(to_string(day) + "," + to_string(near) + "," + to_string(stockPrice) + "," + to_string(stockPrice * near));
    }
}












