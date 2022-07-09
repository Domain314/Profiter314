#ifndef PROFITER3_HELPER_H
#define PROFITER3_HELPER_H

#include <random>

// Normal distribution. a = average, f = fluctuation
static float randomFloat(float a, float f)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::normal_distribution<> dis(a, f);
    return std::fabs((float)dis(gen));
}

// Real distribution. [min - max)
static float randomFloatReal(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(min, max);
    return std::fabs((float)dis(gen));
}

// Real distribution. returns between 0.0 and 1.0
static float randomPercentage()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.000000001);
    return std::fabs((float)dis(gen));
}

// Normal distribution. a = average, f = fluctuation
static int randomInt(int a, int f)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::normal_distribution<> dis(a, f);
    return std::abs((int)dis(gen));
}

// Real distribution. [min - max)
static int randomIntReal(int min, int max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);
    return std::abs((int)dis(gen));
}


#endif //PROFITER3_HELPER_H
