#pragma once

#include <fstream>
#include <string>
#include <string_view>

// Forward declarations
#include "stats.h"
#include "messages.h"

struct CSVLogger
{
    std::ofstream file;

    explicit CSVLogger(std::string_view filename);
    ~CSVLogger();

    template <typename T>
    void write(T &&data)
    {
        if (!file.is_open())
        {
            throw std::runtime_error("Attempting to write to closed CSV file");
        }

        const auto &[ticker, stats] = data();
        writeTickerData(ticker);
        writeStatsData(stats);
        file << "\n";

        file.flush();
        if (file.fail())
        {
            throw std::runtime_error("Failed to write to CSV file");
        }
    }

private:
    void writeHeader();
    void writeTickerHeader();
    void writeStatsHeader();
    void writeTickerData(const Ticker &ticker);
    void writeStatsData(const Stats &stats);
};