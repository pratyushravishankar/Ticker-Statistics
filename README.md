# Ticker Statistics

WebSocket-based cryptocurrency ticker that connects to Coinbase, calculates EMAs of price field and mid-price, and logs data to CSV.

## Requirements

- C++20 compiler
- CMake 3.14 or higher
- OpenSSL
- Boost
- nlohmann_json
- WebSocket++

## Build Instructions


Create build directory
mkdir build && cd build

Configure
cmake ..

Build
cmake --build .

