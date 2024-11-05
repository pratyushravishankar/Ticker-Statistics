#include "ema.h"

EMA::EMA(double timeWindow) : alpha(2.0 / (timeWindow + 1)) {}

void EMA::update(double price)
{
    if (!ema)
    {
        ema = price;
    }
    else
    {
        *ema = (price * alpha) + (*ema * (1 - alpha));
    }
}

double EMA::getValue() const
{
    return ema.value_or(0.0);
}