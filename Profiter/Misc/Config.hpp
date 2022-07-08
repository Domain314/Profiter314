#ifndef PROFITER3_CONFIG_HPP
#define PROFITER3_CONFIG_HPP

#include <string>
#include <vector>

typedef struct Day {
    std::vector<float> hours;
}Day;

typedef struct RandomGenStock {
    float startPrice;
    float endPrice;
    float stockFluctuation;
}RandomGenStock;

typedef struct RandomGenMusician {
    int musicianAmount;
    int musicianFluctuation;
    int daysBetweenReleases;
    int daysFluctuation;
    int amountNFT;
    int amountNFTFluctuation;
    float price;
    float priceFluctuation;
    float fame;
    float fameFluctuation;
    float kryptoAcceptance;
    float kryptoAcceptanceFluctuation;
    float changeFee;

}RandomGenMusician;

typedef struct RandomGenClient {
    int clientAmount;
    int clientFluctuation;
    float capital;
    float capitalFluctuation;
    float motivationToBuy;
    float motivationToBuyFluctuation;
    float motivationToResell;
    float motivationToResellFluctuation;
    float loyalty;
    float loyaltyFluctuation;
    float kryptoAcceptance;
    float kryptoAcceptanceFluctuation;
    int amountMusiciansPerSession;
    int amountMusiciansPerSessionFluctuation;
    float changeFee;

}RandomGenClient;

typedef struct Config {
    // Random Generation
    RandomGenStock randomGenStock;
    RandomGenMusician randomGenMusician;
    RandomGenClient randomGenClient;

    // General Settings
    int days;
    float bsideCut;
    std::string pathToConfigStock;
    std::string pathToConfigMusicians;
    std::string pathToConfigClients;
    std::string pathToConfigBSide;
    std::string mode;       // SMC => Stock Musician Client => 'g' generate | 'l' load
    std::string pathToLoadStock;
    std::string pathToLoadMusicians;
    std::string pathToLoadClients;

}Config;


typedef struct NFT {
    int id;
    float value;
    std::string ownerName;
    int bandID;
    std::string bandName;
    std::vector<int> timeAtWhichWasSold;
    std::vector<int> priceAtWhichWasSold;

    NFT(int _id, float _value, int _bandID, std::string _bandName) {
        id = _id;
        value = _value;
        ownerName = _bandName;
        bandID = _bandID;
        bandName = _bandName;
    }

}NFT;

typedef struct Musician {
    std::string name;
    int id;
    float fame;
    bool kryptoAcceptance;
    std::vector<int> DaysBetweenReleases;
    std::vector<int> NFTsPerRelease;
    std::vector<float> PricesPerRelease;
    std::vector<float> creditHistory;
    std::vector<NFT*> NFTWallet;
}Musician;

typedef struct Client {
    std::string name;
    int id;
    float capital;
    float motivationToBuy;
    float motivationToResell;
    float loyalty;
    bool kryptoAcceptance;
    int dayTimeToCheckMarket;
    int amountMusiciansPerSession;
    std::vector<float> creditHistory;
    std::vector<NFT*> NFTwallet;
}Client;

typedef struct BSide {
    float capitalEUR;
    float capitalNEAR;
    float feeChange;
    float feeBSide;
    float exchangeAmount;

}BSide;




#endif //PROFITER3_CONFIG_HPP
