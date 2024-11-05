#include "messages.h"
#include <nlohmann/json.hpp>
#include <cassert>
#include <iostream>
void testValidJson()
{
    std::string validJson = R"({
        "type": "ticker",
        "sequence": 123456,
        "product_id": "ETH-USD",
        "price": "100.00",
        "best_bid": "99.00",
        "best_ask": "101.00",
        "best_bid_size": "1.5",
        "best_ask_size": "2.0",
        "side": "buy",
        "time": "2024-03-20T10:00:00.000Z",
        "trade_id": 987654,
        "last_size": "1.0",
        "open_24h": "95.00",
        "volume_24h": "1000.0",
        "low_24h": "94.00",
        "high_24h": "105.00",
        "volume_30d": "30000.0"
    })";

    // Test parsing valid JSON to TickerWire
    auto j = nlohmann::json::parse(validJson);
    auto tickerWire = j.get<TickerWire>();

    // Test conversion from wire format to internal Ticker
    auto ticker = Ticker::fromWire(tickerWire);

    assert(ticker.type == "ticker");
    assert(ticker.price == 100.00);
    assert(ticker.best_bid == 99.00);
    assert(ticker.best_ask == 101.00);
    assert(ticker.product_id == "ETH-USD");
}

void testInvalidPrice()
{
    std::string invalidJson = R"({
        "type": "ticker",
        "price": "invalid",
        "best_bid": "99.00",
        "best_ask": "101.00"
    })";

    bool exceptionCaught = false;
    try
    {
        auto invalidJ = nlohmann::json::parse(invalidJson);
        auto invalidTickerWire = invalidJ.get<TickerWire>();
        Ticker::fromWire(invalidTickerWire);
    }
    catch (const std::exception &)
    {
        exceptionCaught = true;
    }
    assert(exceptionCaught);
}

void testMissingFields()
{
    bool exceptionCaught = false;
    std::string missingFieldsJson = R"({
            "type": "ticker"
        })";

    try
    {
        auto missingJ = nlohmann::json::parse(missingFieldsJson);
        auto missingTickerWire = missingJ.get<TickerWire>();
    }
    catch (const nlohmann::json::exception &)
    {
        exceptionCaught = true;
    }
    assert(exceptionCaught);
}

int main()
{
    testValidJson();
    testInvalidPrice();
    testMissingFields();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
