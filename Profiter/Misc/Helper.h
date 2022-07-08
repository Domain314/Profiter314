#ifndef PROFITER3_HELPER_H
#define PROFITER3_HELPER_H

#include <random>

static float randomFloat(float a, float b)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::normal_distribution<> dis(a, b);
//    static std::uniform_real_distribution<> dis(a, b);
    return std::fabs(dis(gen));
}

static float randomPercentage()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
//    std::normal_distribution<> dis(a, b);
    std::uniform_real_distribution<> dis(0.0, 1.000000001);
    return std::fabs(dis(gen));
}

static int randomInt(int a, int b)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::normal_distribution<> dis(a, b);
//    static std::uniform_real_distribution<> dis(a, b);
    return std::abs((int)dis(gen));
}

static int randomIntReal(int min, int max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
//    static std::normal_distribution<> dis(a, b);
    std::uniform_real_distribution<> dis(min, max);
    return std::abs((int)dis(gen));
}

#define randomNum(max) (std::rand()%max)

#define clearScreen() (std::cout << "\033c")


#endif //PROFITER3_HELPER_H
