#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

struct SubscribeMessage
{
    std::string type;
    std::vector<std::string> product_ids;
    std::vector<std::string> channels;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(SubscribeMessage, type, product_ids, channels)
};

// Wire format - matches exactly what Coinbase sends
struct TickerWire
{
    std::string type;
    int64_t sequence;
    std::string product_id;
    std::string price;
    std::string open_24h;
    std::string volume_24h;
    std::string low_24h;
    std::string high_24h;
    std::string volume_30d;
    std::string best_bid;
    std::string best_bid_size;
    std::string best_ask;
    std::string best_ask_size;
    std::string side;
    std::string time;
    int64_t trade_id;
    std::string last_size;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(TickerWire, type, sequence, product_id, price,
                                   open_24h, volume_24h, low_24h, high_24h, volume_30d,
                                   best_bid, best_bid_size, best_ask, best_ask_size,
                                   side, time, trade_id, last_size)
};

// internally used format
struct Ticker
{
    std::string type;
    int64_t sequence;
    std::string product_id;
    double price;
    double open_24h;
    double volume_24h;
    double low_24h;
    double high_24h;
    double volume_30d;
    double best_bid;
    double best_bid_size;
    double best_ask;
    double best_ask_size;
    std::string side;
    std::string time;
    int64_t trade_id;
    double last_size;

    static Ticker fromWire(const TickerWire &wire)
    {
        return Ticker{
            .type = wire.type,
            .sequence = wire.sequence,
            .product_id = wire.product_id,
            .price = std::stod(wire.price),
            .open_24h = std::stod(wire.open_24h),
            .volume_24h = std::stod(wire.volume_24h),
            .low_24h = std::stod(wire.low_24h),
            .high_24h = std::stod(wire.high_24h),
            .volume_30d = std::stod(wire.volume_30d),
            .best_bid = std::stod(wire.best_bid),
            .best_bid_size = std::stod(wire.best_bid_size),
            .best_ask = std::stod(wire.best_ask),
            .best_ask_size = std::stod(wire.best_ask_size),
            .side = wire.side,
            .time = wire.time,
            .trade_id = wire.trade_id,
            .last_size = std::stod(wire.last_size)};
    }
};