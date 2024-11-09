# Ticker Statistics

WebSocket-based cryptocurrency ticker that connects to Coinbase, calculates EMAs of price field and mid-price, and logs data to CSV.
<img width="1678" alt="Screenshot 2024-11-09 at 14 03 00" src="https://github.com/user-attachments/assets/9a852464-9c63-4e8c-b565-3c835f2c99c7">

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

