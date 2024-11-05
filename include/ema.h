#pragma once
#include <optional>

class EMA
{
private:
    std::optional<double> ema;
    double alpha;

public:
    explicit EMA(double timeWindow);
    double getValue() const;
    void update(double price);
};