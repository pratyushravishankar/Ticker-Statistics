#include "csvLogger.h"
#include <iomanip>

CSVLogger::CSVLogger(std::string_view filename)
    : file(filename.data())
{
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open CSV file: " + std::string(filename));
    }
    writeHeader();
}

CSVLogger::~CSVLogger()
{
    if (file.is_open())
    {
        file.close();
    }
}

void CSVLogger::writeHeader()
{
    writeTickerHeader();
    writeStatsHeader();
    file << "\n";
}

void CSVLogger::writeTickerHeader()
{
    file << "time,"
         << "sequence,"
         << "product_id,"
         << "price,"
         << "open_24h,"
         << "volume_24h,"
         << "low_24h,"
         << "high_24h,"
         << "volume_30d,"
         << "best_bid,"
         << "best_bid_size,"
         << "best_ask,"
         << "best_ask_size,"
         << "side,"
         << "trade_id,"
         << "last_size";
}

void CSVLogger::writeStatsHeader()
{
    file << ",price_ema"
         << ",mid_price_ema";
}

void CSVLogger::writeTickerData(const Ticker &ticker)
{
    file << ticker.time << ","
         << ticker.sequence << ","
         << ticker.product_id << ","
         << ticker.price << ","
         << ticker.open_24h << ","
         << ticker.volume_24h << ","
         << ticker.low_24h << ","
         << ticker.high_24h << ","
         << ticker.volume_30d << ","
         << ticker.best_bid << ","
         << ticker.best_bid_size << ","
         << ticker.best_ask << ","
         << ticker.best_ask_size << ","
         << ticker.side << ","
         << ticker.trade_id << ","
         << ticker.last_size;
}

void CSVLogger::writeStatsData(const Stats &stats)
{
    file << std::fixed << std::setprecision(2)
         << "," << stats.priceEMA
         << "," << stats.midPriceEMA;
}