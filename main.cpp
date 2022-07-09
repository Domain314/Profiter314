#include <iostream>
#include "Profiter/Profit.hpp"

void TESTING() {
//    vector<int> counter;
//    for (int i = 0; i < 50; ++i) {
//        counter.push_back(0);
//    }
//    for (int i = 0; i < 2000; ++i) {
//        int rnd = randomInt(16, 4)%24;
//        cout << rnd << "\n";
//        counter.at(rnd) += 1;
//    }
//    for (int i = 0; i < 50; ++i) {
//        cout << i << ": " << counter.at(i) << " ";
//        for (int j = 0; j < counter.at(i); ++j) {
//            cout << "*";
//        }
//        cout << "\n";
//    }

//    for (int i = 0; i < 10000; ++i) {
//        cout << randomPercentage() << "\n";
//    }
}

int main() {

//    TESTING();

    srand(time(NULL));
    Profit* profit = new Profit();
    delete profit;

    return 0;
}
