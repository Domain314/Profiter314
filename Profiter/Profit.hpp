#ifndef PROFITER3_PROFIT_HPP
#define PROFITER3_PROFIT_HPP


#include "Holder.hpp"
#include "TimeEngine.hpp"

class Profit {
public:
    Profit();
    ~Profit();

private:
    Holder* holder;
    TimeEngine* timeEngine;
};


#endif //PROFITER3_PROFIT_HPP
