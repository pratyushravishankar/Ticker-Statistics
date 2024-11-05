#include "stats.h"
#include <cmath>

double roundTo2DP(double value)
{
    return std::round(value * 100.0) / 100.0;
}