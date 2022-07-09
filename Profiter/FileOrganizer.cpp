#include "FileOrganizer.hpp"

// Destructor
FileOrganizer::~FileOrganizer() { }

// Constructor
FileOrganizer::FileOrganizer() { }


// Read config.txt and save as map.
// Fill up all config-settings.
// return this config.
Config* FileOrganizer::createConfig(BSide* bside) {
    map<string,string> configMap;
    if(readAsMap(&configMap)) {
        cout << "Config loaded successfully\n";
    } else {
        cout << "Config Error. Shutting down..\n";
        return nullptr;
    }

    config = new Config();

    config->mode = configMap.at("mode");
    config->days = stoi(configMap.at("days"));
    config->bsideCut = stof(configMap.at("BSide_Cut"));;
    config->pathToLoadStock = configMap.at("Stock_Save_Path");
    config->pathToLoadMusicians = configMap.at("Musician_Save_Path");
    config->pathToLoadClients = configMap.at("Client_Save_Path");

    config->randomGenStock.startPrice = stof(configMap.at("Start_Price"));
    config->randomGenStock.endPrice = stof(configMap.at("End_Price"));
    config->randomGenStock.stockFluctuation = stof(configMap.at("Stock_Fluctuation"));

    config->randomGenMusician.musicianAmount = stoi(configMap.at("Amount_Musician"));
    config->randomGenMusician.musicianFluctuation = stoi(configMap.at("Amount_Musician_Fluctuation"));
    config->randomGenMusician.daysBetweenReleases = stoi(configMap.at("Days_Between_Releases"));
    config->randomGenMusician.daysFluctuation = stoi(configMap.at("Days_Between_Releases_Fluctuation"));
    config->randomGenMusician.amountNFT = stoi(configMap.at("NFTs_Per_Release"));
    config->randomGenMusician.amountNFTFluctuation = stoi(configMap.at("NFTs_Per_Release_Fluctuation"));
    config->randomGenMusician.price = stof(configMap.at("Sell_Price"));
    config->randomGenMusician.priceFluctuation = stof(configMap.at("Sell_Price_Fluctuation"));
    config->randomGenMusician.fame = stof(configMap.at("Popularity"));
    config->randomGenMusician.fameFluctuation = stof(configMap.at("Popularity_Fluctuation"));
    config->randomGenMusician.kryptoAcceptance = stof(configMap.at("Krypto_Acceptance_Musician"));
    config->randomGenMusician.kryptoAcceptanceFluctuation = stof(configMap.at("Krypto_Acceptance_Musician_Fluctuation"));
    config->randomGenMusician.changeFee = stof(configMap.at("NEAR_EUR_Change_Fee_Musician"));

    config->randomGenClient.clientAmount = stoi(configMap.at("Amount_Client"));
    config->randomGenClient.clientFluctuation = stoi(configMap.at("Amount_Client_Fluctuation"));
    config->randomGenClient.capital = stof(configMap.at("Capital_Client"));
    config->randomGenClient.capitalFluctuation = stof(configMap.at("Capital_Client_Fluctuation"));
    config->randomGenClient.motivationToBuy = stof(configMap.at("Buy_Motivation"));
    config->randomGenClient.motivationToBuyFluctuation = stof(configMap.at("Buy_Motivation_Fluctuation"));
    config->randomGenClient.motivationToResell = stof(configMap.at("Resell_Motivation"));
    config->randomGenClient.motivationToResellFluctuation = stof(configMap.at("Resell_Motivation_Fluctuation"));
    config->randomGenClient.loyalty = stof(configMap.at("Loyalty"));
    config->randomGenClient.loyaltyFluctuation = stof(configMap.at("Loyalty_Fluctuation"));
    config->randomGenClient.kryptoAcceptance = stof(configMap.at("Krypto_Acceptance_Client"));
    config->randomGenClient.kryptoAcceptanceFluctuation = stof(configMap.at("Krypto_Acceptance_Client_Fluctuation"));
    config->randomGenClient.amountMusiciansPerSession = stoi(configMap.at("Musicians_Per_Day"));
    config->randomGenClient.amountMusiciansPerSessionFluctuation = stoi(configMap.at("Musicians_Per_Day_Fluctuation"));
    config->randomGenClient.changeFee = stof(configMap.at("NEAR_EUR_Change_Fee_Client"));

    bside->capitalEUR = stof(configMap.at("Capital_EUR"));
    bside->capitalNEAR = stof(configMap.at("Capital_NEAR"));
    bside->exchangeAmount = stof(configMap.at("Max_Exchange_Per_Hour"));

    return config;
}

// Generate random stock history, based on config input
// Create vector<Day*> and calculate start and end price of this day (LERP)
// Fill each day with 24 prices for each hour.
// Random fluctuation will be added/subtracted based on the price from lastHour.
// If lastHour is greater than the end price of the day, the fluctuation will be subtracted, else vice versa
// Write result to disk (.csv)
vector<Day*>* FileOrganizer::randomStocksFromConfig() {
    vector<Day*>* days= new vector<Day*>();
    float t = 1/(float)config->days;
    for (int i = 0; i < config->days; ++i) {
        days->push_back(new Day());
        float dayStartPrice = lerp(config->randomGenStock.startPrice, config->randomGenStock.endPrice, t*(i));
        float dayEndPrice = lerp(config->randomGenStock.startPrice, config->randomGenStock.endPrice, t*(i+1));
        days->at(i)->hours.push_back(dayStartPrice);
        for (int j = 1; j < 24; ++j) {
            float fluctuation = randomFloatReal(0, config->randomGenStock.stockFluctuation);
            float lastHour = days->at(i)->hours.at(j-1);
            if (lastHour < dayEndPrice) {
                days->at(i)->hours.push_back(lastHour + fluctuation);
            } else {
                days->at(i)->hours.push_back(lastHour - fluctuation);
            }

        }
    }
    saveStocksAsCSV("RESULT_GEN/RandomStock", days);
    cout << "Stock generated and saved." << endl;
    return days;
}

// Generate random musicians, based on config input.
// Band name and ID are picked randomly with a flat distribution.
// (Every possible name/number has the same chance)
// All other random numbers have a normal curve-distribution.
// The first value gives is the avarage, while the second (± Fluctuation) gives the Quartile
// (NOT MIN/MAX. Some values may fall even deeper/grow higher)
// Write result to disk (.csv)
vector<Musician*>* FileOrganizer::randomMusiciansFromConfig() {
    vector<string> bandNames = readFromTXTFile("RandomNames/bandNames.txt", false);
    RandomGenMusician* randomGenMusician = &config->randomGenMusician;
    vector<Musician*>* musicians = new vector<Musician*>();
    for (int i = 0; i < randomGenMusician->musicianAmount; ++i) {
        Musician* musician = new Musician();

        musician->name = bandNames.at(randomIntReal(0, bandNames.size()));
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
    cout << "Musicians generated and saved." << endl;
    return musicians;
}

// (Same as musicians)
// Generate random clients, based on config input.
// Client name and ID are picked randomly with a flat distribution.
// (Every possible name/number has the same chance)
// All other random numbers have a normal curve-distribution.
// The first value gives is the avarage, while the second (± Fluctuation) gives the Quartile
// (NOT MIN/MAX. Some values may fall even deeper/grow higher)
// Write result to disk (.csv)
vector<Client*>* FileOrganizer::randomClientsFromConfig() {
    vector<string> clientNames = readFromTXTFile("RandomNames/clientNames.txt", false);
    RandomGenClient* randomGenClient = &config->randomGenClient;
    vector<Client*>* clients = new vector<Client*>();
    for (int i = 0; i < randomGenClient->clientAmount; ++i) {
        Client* client = new Client();
        client->name = clientNames.at(randomIntReal(0,clientNames.size()));
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
    cout << "Clients generated and saved." << endl;
    return clients;
}

// Write stock data to disk (.csv)
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

// Write musicians data to disk (.csv)
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

// Write clients data to disk (.csv)
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

// Read config.txt
// If line starts with '#' oder is empty, ignore it.
// If not, break the line at the '=' and save 2 strings in a vector<string>.
// One of them is the key (before '='), the other is the value ('=' after).
// Both get added as a pair<string,string> to the configMap.
// return 1 = true, when everything went well.
int FileOrganizer::readAsMap(map<string, string> *configMap) {
    ifstream inFile("config.txt", ios::in);
    if (inFile.fail()) {
        cerr << "Error: Config not found or not able to open" << endl;
        return 0;
    }
    string lineStr;

    while (std::getline(inFile, lineStr)) {
        if (lineStr[0] == '#' || lineStr == "") { continue; }
        stringstream ss(lineStr);
        vector<string> strArray;
        for (int i = 0; i < 2; ++i) {
            getline(ss, lineStr, '=');
            strArray.push_back(lineStr);
        }
        configMap->insert(pair<string,string>(strArray.at(0), strArray.at(1)));
    }
    inFile.close();
    return 1;
}

// Read stock data from disk.
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

// Read musicians data from disk.
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

// Read clients data from disk.
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

// Read data from a .txt file (for band-/clients names)
vector<string> FileOrganizer::readFromTXTFile(string path, bool config) {
    ifstream inFile(path, ios::in);
    if (inFile.fail()) {
        cerr << "Error: File not found or not able to open: " << path << endl;
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

// Read data from a .csv file. (for already generated Stocks/Musicians/Clients)
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

// Save results of simulation to disk (.csv)
void FileOrganizer::saveSimToCSV(vector<string> *exchanges, vector<string> *NEAR_EUR, vector<string> *EUR_NEAR,
                                 vector<string> *nftsSold) {
    writeVectorStringToCSV("A_EXCHANGES", exchanges, "Day,NEAR,Stock Price,EUR");
    writeVectorStringToCSV("A_NEAR_EUR", NEAR_EUR, "Day,Max Musician Profit,Change Fee,Profit after Fee,Profit in €");
    writeVectorStringToCSV("A_EUR_NEAR", EUR_NEAR, "Day,Original Price,Change Fee,Price after Fee,Price in €");
    writeVectorStringToCSV("A_NFTS_SOLD", nftsSold, "Day,ID,Musician,M KL,Client,C KL,Price,Client Price,Musician Profit,B-Side Profit");
}

// Write content of a vector<string> to disk (.csv)
void FileOrganizer::writeVectorStringToCSV(string name, vector<string>* vecStr, string headline) {
    ofstream newFile;
    newFile.open("RESULT_SIM/" + name + ".csv");
    newFile << headline << "\n";
    for (int i = 0; i < vecStr->size(); ++i) {
        newFile << vecStr->at(i) << ",\n";
    }
    newFile.close();
}

// Write bside results to disk (.csv)
void FileOrganizer::writeBsideResult(float NEAR, float EUR) {
    ofstream newFile;
    newFile.open("RESULT_SIM/BSide.txt");
    newFile << "NEAR: " << to_string(NEAR) << "\n";
    newFile << "EUR: " << to_string(EUR) << "\n";
    newFile.close();
}

