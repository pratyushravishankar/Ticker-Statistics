#include "statsTracker.h"
#include <iostream>
#include <cassert>
#include <string_view>

struct MockWriter
{
    template <typename F>
    void write(F &&f)
    {
    }
    MockWriter(std::string_view) {};
};

constexpr std::string_view TEST_FILENAME = "test.csv";

auto getTracker()
{
    return StatsTracker<MockWriter>{5.0, TEST_FILENAME};
}

void testEMAMidPrice()
{
    auto statsTracker = getTracker();
    statsTracker.onTickerReceived(Ticker{.best_bid = 108, .best_ask = 112, .time = "0"});
    statsTracker.onTickerReceived(Ticker{.best_bid = 130, .best_ask = 150, .time = "1"});
    statsTracker.onTickerReceived(Ticker{.best_bid = 245, .best_ask = 255, .time = "2"});
    statsTracker.onTickerReceived(Ticker{.best_bid = 299, .best_ask = 301, .time = "7"});
    assert(statsTracker.getStats().midPriceEMA == 208.89);
}

void testEMAPrice()
{
    auto statsTracker = getTracker();
    statsTracker.onTickerReceived(Ticker{.price = 100, .time = "0"});
    statsTracker.onTickerReceived(Ticker{.price = 140, .time = "1"});
    statsTracker.onTickerReceived(Ticker{.price = 250, .time = "2"});
    statsTracker.onTickerReceived(Ticker{.price = 300, .time = "7"});
    assert(statsTracker.getStats().priceEMA == 205.93);
}

void testNoTickers()
{
    auto statsTracker = getTracker();
    auto stats = statsTracker.getStats();
    assert(stats.priceEMA == 0.0);
    assert(stats.midPriceEMA == 0.0);
}

void testEMA()
{
    auto statsTracker = getTracker();
    statsTracker.onTickerReceived(Ticker{.best_ask = 99, .price = 100, .best_bid = 101, .time = "0"});
    statsTracker.onTickerReceived(Ticker{.best_ask = 120, .price = 141, .best_bid = 150, .time = "1"});
    statsTracker.onTickerReceived(Ticker{.best_ask = 245, .price = 251, .best_bid = 255, .time = "2"});
    statsTracker.onTickerReceived(Ticker{.best_ask = 299, .price = 299, .best_bid = 301, .time = "7"});
    auto stats = statsTracker.getStats();
    assert(stats.midPriceEMA == 205.19);
    assert(stats.priceEMA == 205.96);
}

int main()
{
    testEMAMidPrice();
    testEMAPrice();
    testNoTickers();
    testEMA();
    std::cout << "All tests passed" << std::endl;
    return 0;
}
