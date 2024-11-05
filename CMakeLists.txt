cmake_minimum_required(VERSION 3.14)
project(crypto_ticker VERSION 1.0)

# Basic settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required dependencies
find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(Threads REQUIRED)

# WebSocket++ headers
find_path(WEBSOCKETPP_INCLUDE_DIR websocketpp/client.hpp 
    REQUIRED)

# Source files
set(SOURCES
    src/connectionHandler.cpp
    src/csvLogger.cpp
    src/ema.cpp
    src/stats.cpp
)

# Main executable
add_executable(cryptoTicker 
    src/main.cpp
    ${SOURCES}
)

# Test executable
add_executable(testStatsTracker
    tests/testStatsTracker.cpp
    ${SOURCES}
)

add_executable(testJsonParsing
    tests/testJsonParsing.cpp
    ${SOURCES}
)

# Configure targets
foreach(target cryptoTicker testStatsTracker testJsonParsing)
    target_include_directories(${target} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${WEBSOCKETPP_INCLUDE_DIR}
        ${OPENSSL_INCLUDE_DIR}
        ${Boost_INCLUDE_DIRS}
    )

    target_link_libraries(${target} PRIVATE
        OpenSSL::SSL
        OpenSSL::Crypto
        ${Boost_LIBRARIES}
        nlohmann_json::nlohmann_json
        Threads::Threads
    )
endforeach()
