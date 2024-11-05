#pragma once
#include <array>
#include <string_view>

struct Config
{
    static constexpr int EMA_WINDOW = 5; // seconds
    static constexpr std::string_view CSV_FILENAME = "trading_data.csv";
    static constexpr std::string_view WEBSOCKET_URI = "wss://ws-feed.exchange.coinbase.com";
    static constexpr std::string_view PRODUCT_ID = "ETH-USD";
};
