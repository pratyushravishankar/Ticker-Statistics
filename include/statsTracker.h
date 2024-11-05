#pragma once

#include <string_view>
#include "ema.h"
#include "stats.h"
#include "messages.h"

template <typename Writer>
class StatsTracker
{
private:
    EMA priceEMA;
    EMA midPriceEMA;
    Writer writer;

public:
    StatsTracker(double emaWindow, std::string_view filename);
    Stats getStats() const;
    void onTickerReceived(const Ticker &latestTicker);
};

template <typename Writer>
StatsTracker<Writer>::StatsTracker(double emaWindow, std::string_view filename)
    : priceEMA{emaWindow}, midPriceEMA{emaWindow}, writer{filename} {}

template <typename Writer>
Stats StatsTracker<Writer>::getStats() const
{
    return {
        .priceEMA = roundTo2DP(priceEMA.getValue()),
        .midPriceEMA = roundTo2DP(midPriceEMA.getValue())};
}

template <typename Writer>
void StatsTracker<Writer>::onTickerReceived(const Ticker &latestTicker)
{
    priceEMA.update(latestTicker.price);

    const auto midPrice = (latestTicker.best_bid + latestTicker.best_ask) / 2.0;
    midPriceEMA.update(midPrice);

    writer.write([&]()
                 { return std::make_pair(latestTicker, getStats()); });
}
