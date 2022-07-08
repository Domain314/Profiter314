#include "FileOrganizer.hpp"

FileOrganizer::~FileOrganizer() {
}

FileOrganizer::FileOrganizer() {
}

Config* FileOrganizer::createConfig(BSide* bside) {
    vector<string> rawConfig = readFromTXTFile("config.txt", true);
    if (rawConfig.empty()) {
        cout << "Config not found" << endl;
        return nullptr;
    }

    config = new Config();
    config->pathToConfigStock = rawConfig.at(1);
    config->pathToConfigMusicians = rawConfig.at(2);
    config->pathToConfigClients = rawConfig.at(3);
    config->pathToConfigBSide = rawConfig.at(4);
    config->mode = rawConfig.at(5);
    config->days = stoi(rawConfig.at(6));
    config->bsideCut = stof(rawConfig.at(7));;
    config->pathToLoadStock = rawConfig.at(8);
    config->pathToLoadMusicians = rawConfig.at(9);
    config->pathToLoadClients = rawConfig.at(10);

    vector<string> stockConfig = readFromTXTFile(config->pathToConfigStock, true);
    if (stockConfig.empty()) {
        cout << "Stock config not found" << endl;
        return nullptr;
    }
    config->randomGenStock.startPrice = stof(stockConfig.at(1));
    config->randomGenStock.endPrice = stof(stockConfig.at(2));
    config->randomGenStock.stockFluctuation = stof(stockConfig.at(3));

    vector<string> musicianConfig = readFromTXTFile(config->pathToConfigMusicians, true);
    if (musicianConfig.empty()) {
        cout << "Musician config not found" << endl;
        return nullptr;
    }
    config->randomGenMusician.musicianAmount = stoi(musicianConfig.at(1));
    config->randomGenMusician.musicianFluctuation = stoi(musicianConfig.at(2));
    config->randomGenMusician.daysBetweenReleases = stoi(musicianConfig.at(3));
    config->randomGenMusician.daysFluctuation = stoi(musicianConfig.at(4));
    config->randomGenMusician.amountNFT = stoi(musicianConfig.at(5));
    config->randomGenMusician.amountNFTFluctuation = stoi(musicianConfig.at(6));
    config->randomGenMusician.price = stof(musicianConfig.at(7));
    config->randomGenMusician.priceFluctuation = stof(musicianConfig.at(8));
    config->randomGenMusician.fame = stof(musicianConfig.at(9));
    config->randomGenMusician.fameFluctuation = stof(musicianConfig.at(10));
    config->randomGenMusician.kryptoAcceptance = stof(musicianConfig.at(11));
    config->randomGenMusician.kryptoAcceptanceFluctuation = stof(musicianConfig.at(12));
    config->randomGenMusician.changeFee = stof(musicianConfig.at(13));

    vector<string> clientConfig = readFromTXTFile(config->pathToConfigClients, true);
    if (clientConfig.empty()) {
        cout << "Client config not found" << endl;
        return nullptr;
    }
    config->randomGenClient.clientAmount = stoi(clientConfig.at(1));
    config->randomGenClient.clientFluctuation = stoi(clientConfig.at(2));
    config->randomGenClient.capital = stof(clientConfig.at(3));
    config->randomGenClient.capitalFluctuation = stof(clientConfig.at(4));
    config->randomGenClient.motivationToBuy = stof(clientConfig.at(5));
    config->randomGenClient.motivationToBuyFluctuation = stof(clientConfig.at(6));
    config->randomGenClient.motivationToResell = stof(clientConfig.at(7));
    config->randomGenClient.motivationToResellFluctuation = stof(clientConfig.at(8));
    config->randomGenClient.loyalty = stof(clientConfig.at(9));
    config->randomGenClient.loyaltyFluctuation = stof(clientConfig.at(10));
    config->randomGenClient.kryptoAcceptance = stof(clientConfig.at(11));
    config->randomGenClient.kryptoAcceptanceFluctuation = stof(clientConfig.at(12));
    config->randomGenClient.amountMusiciansPerSession = stoi(clientConfig.at(13));
    config->randomGenClient.amountMusiciansPerSessionFluctuation = stoi(clientConfig.at(14));
    config->randomGenClient.changeFee = stof(clientConfig.at(15));

    vector<string> bsideConfig = readFromTXTFile(config->pathToConfigBSide, true);
    if (clientConfig.empty()) {
        cout << "BSide config not found" << endl;
        return nullptr;
    }
    bside->capitalEUR = stof(bsideConfig.at(1));
    bside->capitalNEAR = stof(bsideConfig.at(2));
    bside->feeChange = stof(bsideConfig.at(3));
    bside->feeBSide = stof(bsideConfig.at(4));
    bside->exchangeAmount = stof(bsideConfig.at(5));

    return config;
}

vector<Day*>* FileOrganizer::randomStocksFromConfig() {
    vector<Day*>* days= new vector<Day*>();
    float t = 1/(float)config->days;
    for (int i = 0; i < config->days; ++i) {
        days->push_back(new Day());
        float dayStartPrice = lerp(config->randomGenStock.startPrice, config->randomGenStock.endPrice, t*(i));
        float dayEndPrice = lerp(config->randomGenStock.startPrice, config->randomGenStock.endPrice, t*i+1);
        days->at(i)->hours.push_back(dayStartPrice);
        for (int j = 1; j < 24; ++j) {
            float fluctuation = randomFloat(0, config->randomGenStock.stockFluctuation);
            float lastHour = days->at(i)->hours.at(j-1);
            if (lastHour + fluctuation < dayEndPrice) {
                days->at(i)->hours.push_back(lastHour + fluctuation);
            } else {
                days->at(i)->hours.push_back(lastHour - fluctuation);
            }

        }
    }
    saveStocksAsCSV("RESULT_GEN/RandomStock", days);
    cout << "saved." << endl;
    return days;
}

vector<Musician*>* FileOrganizer::randomMusiciansFromConfig() {
    vector<string> bandNames = readFromTXTFile("RandomNames/bandNames.txt", false);
    RandomGenMusician* randomGenMusician = &config->randomGenMusician;
    vector<Musician*>* musicians = new vector<Musician*>();
    for (int i = 0; i < randomGenMusician->musicianAmount; ++i) {
        Musician* musician = new Musician();

        musician->name = bandNames.at(randomNum(bandNames.size()));
        musician->id = randomIntReal(1000000, 9999999);
        musician->fame = randomFloat(randomGenMusician->fame, randomGenMusician->fameFluctuation);
        musician->kryptoAcceptance = randomFloat(randomGenMusician->kryptoAcceptance, randomGenMusician->kryptoAcceptanceFluctuation) < 0.5;
        int dayspassed = 0;
        int releaseFrequency = randomInt(randomGenMusician->daysBetweenReleases, randomGenMusician->daysFluctuation);
        int nftFrequency = randomInt(randomGenMusician->amountNFT, randomGenMusician->amountNFTFluctuation);
        int priceFrequency = randomInt(randomGenMusician->price, randomGenMusician->priceFluctuation);
        musician->DaysBetweenReleases.push_back(0);
        musician->NFTsPerRelease.push_back(nftFrequency);
        musician->PricesPerRelease.push_back(priceFrequency);
        while (dayspassed < config->days) {
//            if (dayspassed <= 0) { abs(dayspassed-1); }
            musician->DaysBetweenReleases.push_back(releaseFrequency);
            musician->NFTsPerRelease.push_back(nftFrequency);
            musician->PricesPerRelease.push_back(priceFrequency);
            dayspassed += releaseFrequency;
        }
        musicians->push_back(musician);
    }
    saveMusiciansAsCSV("RESULT_GEN/RandomMusicians", musicians);
    cout << "saved." << endl;
    return musicians;
}

vector<Client*>* FileOrganizer::randomClientsFromConfig() {
    vector<string> clientNames = readFromTXTFile("RandomNames/clientNames.txt", false);
    RandomGenClient* randomGenClient = &config->randomGenClient;
    vector<Client*>* clients = new vector<Client*>();
    for (int i = 0; i < randomGenClient->clientAmount; ++i) {
        Client* client = new Client();
        client->name = clientNames.at(randomNum(clientNames.size()));
        client->id = randomIntReal(1000000, 9999999);
        client->capital = randomFloat(randomGenClient->capital, randomGenClient->capitalFluctuation);
        client->motivationToBuy = randomFloat(randomGenClient->motivationToBuy, randomGenClient->motivationToBuyFluctuation);
        client->motivationToResell = randomFloat(randomGenClient->motivationToResell, randomGenClient->motivationToResellFluctuation);
        client->loyalty = randomFloat(randomGenClient->loyalty, randomGenClient->loyaltyFluctuation);
        client->kryptoAcceptance = randomFloat(randomGenClient->kryptoAcceptance, randomGenClient->kryptoAcceptanceFluctuation) < 0.5;
        client->dayTimeToCheckMarket = randomInt(16, 4)%24;
        client->amountMusiciansPerSession = randomInt(randomGenClient->amountMusiciansPerSession, randomGenClient->amountMusiciansPerSessionFluctuation);

        clients->push_back(client);
    }
    saveClientsAsCSV("RESULT_GEN/RandomClients", clients);
    cout << "saved." << endl;
    return clients;
}

void FileOrganizer::saveStocksAsCSV(string name, vector<Day*>* days) {
    ofstream newFile;
    newFile.open(name + ".csv");
    for (int i = 0; i < days->size(); ++i) {
        for (int j = 0; j < 24; ++j) {
            newFile << days->at(i)->hours.at(j) << ",\n";
        }
    }
    newFile.close();
}

void FileOrganizer::saveMusiciansAsCSV(string name, vector<Musician*>* musicians) {
    ofstream newFile;
    newFile.open(name + ".csv");
    newFile << "ID,Name,Fame,KryptoLove,Release Freq,NFT Freq,Price Freq,Credit History,NFT Wallet,\n";
    for (int i = 0; i < musicians->size(); ++i) {
        Musician* musician = musicians->at(i);
        newFile << musician->id << ",";
        newFile << musician->name << ",";
        newFile << musician->fame << ",";
        newFile << musician->kryptoAcceptance << ",";
        for (int j = 0; j < musician->DaysBetweenReleases.size(); ++j) {
            newFile << musician->DaysBetweenReleases.at(j) << " ";
        }
        newFile << ",";
        for (int j = 0; j < musician->NFTsPerRelease.size(); ++j) {
            newFile << musician->NFTsPerRelease.at(j) << " ";
        }
        newFile << ",";
        for (int j = 0; j < musician->PricesPerRelease.size(); ++j) {
            newFile << musician->PricesPerRelease.at(j) << " ";
        }
        newFile << ",";
        for (int j = 0; j < musician->creditHistory.size(); ++j) {
            newFile << musician->creditHistory.at(j) << " ";
        }
        newFile << ",";
        newFile << musician->NFTWallet.size() << ": ";
        for (int j = 0; j < musician->NFTWallet.size(); ++j) {
            newFile << musician->NFTWallet.at(j)->id << " ";
        }
        newFile << ",\n";
    }
    newFile.close();
}

void FileOrganizer::saveClientsAsCSV(string name, vector<Client*>* clients) {
    ofstream newFile;
    newFile.open(name + ".csv");
    newFile << "ID,Name,Capital,Buy Motivation,Resell Motivation,Loyalty,KrpytoLove,Daytime,Musicians Per Session,Credit History,NFT Wallet,\n";
    for (int i = 0; i < clients->size(); ++i) {
        Client* client = clients->at(i);
        newFile << client->id << ",";
        newFile << client->name << ",";
        newFile << client->capital << ",";
        newFile << client->motivationToBuy << ",";
        newFile << client->motivationToResell << ",";
        newFile << client->loyalty << ",";
        newFile << client->kryptoAcceptance << ",";
        newFile << client->dayTimeToCheckMarket << ",";
        newFile << client->amountMusiciansPerSession << ",";
        for (int j = 0; j < client->creditHistory.size(); ++j) {
            newFile << client->creditHistory.at(j) << " ";
        }
        newFile << ",";
        newFile << client->NFTwallet.size() << ": ";
        for (int j = 0; j < client->NFTwallet.size(); ++j) {
            newFile << client->NFTwallet.at(j)->id << " ";
        }
        newFile << ",\n";
    }
    newFile.close();
}

vector<Day*>* FileOrganizer::loadStocksFromCSV() {
    vector<string> strArray = readFromCSVFile(config->pathToLoadStock, 1);
    vector<Day*>* days = new vector<Day*>();
    for (int i = 0; i < strArray.size(); i+=24) {
        Day* day = new Day();
        for (int j = 0; j < 24; ++j) {
            day->hours.push_back(stof(strArray.at(i+j)));
        }
        days->push_back(day);
    }
    return days;
}

vector<Musician*>* FileOrganizer::loadMusiciansFromCSV() {
    vector<string> strArray = readFromCSVFile(config->pathToLoadMusicians, 9);
    vector<Musician*>* musicians = new vector<Musician*>();
    for (int i = 9; i < strArray.size(); ++i) {
        Musician* musician = new Musician();
        musician->id = stoi(strArray.at(i++));
        musician->name = strArray.at(i++);
        musician->fame = stof(strArray.at(i++));
        musician->kryptoAcceptance = strArray.at(i++) == "1";
        string lineStr;
        stringstream ss;
        ss << strArray.at(i++);
        while(getline(ss, lineStr, ' ')) {
            musician->DaysBetweenReleases.push_back(stoi(lineStr));
        }
        ss.clear();
        ss << strArray.at(i++);
        while(getline(ss, lineStr, ' ')) {
            musician->NFTsPerRelease.push_back(stoi(lineStr));
        }
        ss.clear();
        ss << strArray.at(i++);
        while(getline(ss, lineStr, ' ')) {
            musician->PricesPerRelease.push_back(stof(lineStr));
        }
        ss.clear();
        ss << strArray.at(i++);
        while(getline(ss, lineStr, ' ')) {
            musician->creditHistory.push_back(stof(lineStr));
        }
        musicians->push_back(musician);
    }
    return musicians;
}

vector<Client*>* FileOrganizer::loadClientsFromCSV() {
    vector<string> strArray = readFromCSVFile(config->pathToLoadClients, 11);
    vector<Client*>* clients = new vector<Client*>();
    for (int i = 11; i < strArray.size(); ++i) {
        Client* client = new Client();
        client->id = stoi(strArray.at(i++));
        client->name = strArray.at(i++);
        client->capital = stof(strArray.at(i++));
        client->motivationToBuy = stof(strArray.at(i++));
        client->motivationToResell = stof(strArray.at(i++));
        client->loyalty = stof(strArray.at(i++));
        client->kryptoAcceptance = stof(strArray.at(i++));
        client->dayTimeToCheckMarket = stoi(strArray.at(i++));
        client->amountMusiciansPerSession = stoi(strArray.at(i++));
        string lineStr;
        stringstream ss;
        ss << strArray.at(i++);
        while(getline(ss, lineStr, ' ')) {
            client->creditHistory.push_back(stof(lineStr));
        }
        ss.clear();
        ss << strArray.at(i);
        while(getline(ss, lineStr, ' ')) {
            client->creditHistory.push_back(stof(lineStr));
        }
        clients->push_back(client);
    }
    cout << "done";
    return clients;
}

vector<string> FileOrganizer::readFromTXTFile(string path, bool config) {
    ifstream inFile(path, ios::in);
    if (inFile.fail()) {
        cerr << "Error: File not found or not able to open" << endl;
        return vector<string>();
    }
    std::cout << "loading File from " << path << "\n";
    string lineStr;
    vector<string> strArray;
    if (config) {
        strArray.push_back("#Config"); // unnecessary line for better readable code (index = line number in .txt)
    }
    while (std::getline(inFile, lineStr)) {
        strArray.push_back(lineStr);
    }
    inFile.close();
    return strArray;
}

vector<string> FileOrganizer::readFromCSVFile(string path, int columns) {
    ifstream inFile(path, ios::in);
    if (inFile.fail()) {
        cerr << "Error: File not found or not able to open" << endl;
        return vector<string>();
    }
    std::cout << "loading File from " << path << "\n";
    string lineStr;
    vector<string> strArray;
    while (std::getline(inFile, lineStr)) {
        stringstream ss(lineStr);
        for (int i = 0; i < columns; ++i) {
            getline(ss, lineStr, ',');
            strArray.push_back(lineStr);
        }
    }
    inFile.close();
    return strArray;
}

void FileOrganizer::saveSimToCSV(vector<string> *exchanges, vector<string> *NEAR_EUR, vector<string> *EUR_NEAR,
                                 vector<string> *nftsSold) {
    writeVectorStringToCSV("A_EXCHANGES", exchanges, "NEAR,StockPrice,EUR");
    writeVectorStringToCSV("A_NEAR_EUR", NEAR_EUR, "Day,NEAR,EUR");
    writeVectorStringToCSV("A_EUR_NEAR", EUR_NEAR, "Day,EUR,NEAR");
    writeVectorStringToCSV("A_NFTS_SOLD", nftsSold, "Day,ID,Band,Client,Price");
}

void FileOrganizer::writeVectorStringToCSV(string name, vector<string>* vecStr, string headline) {
    ofstream newFile;
    newFile.open("RESULT_SIM/" + name + ".csv");
    newFile << headline << "\n";
    for (int i = 0; i < vecStr->size(); ++i) {
        newFile << vecStr->at(i) << ",\n";
    }
    newFile.close();
}

void FileOrganizer::writeBsideResult(float NEAR, float EUR) {
    ofstream newFile;
    newFile.open("RESULT_SIM/BSide.txt");
    newFile << "NEAR: " << to_string(NEAR) << "\n";
    newFile << "EUR: " << to_string(EUR) << "\n";
    newFile.close();
}


