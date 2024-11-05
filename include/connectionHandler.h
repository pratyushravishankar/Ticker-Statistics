#pragma once

#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_client.hpp>
#include <optional>
#include <string_view>
#include "statsTracker.h"
#include "csvLogger.h"
#include "config.h"


class ConnectionHandler
{
private:
    using Client = websocketpp::client<websocketpp::config::asio_tls_client>;
    using ConnectionPtr = websocketpp::connection_hdl;

    Client client;
    ConnectionPtr connection;
    StatsTracker<CSVLogger> statsTracker;

    void setupHandlers();
    void subscribe();
    void onSubscriptionSuccess();
    void onMessage(std::string msg);
    std::optional<std::string> getMsgType(const nlohmann::json &j);
    void sendMessage(std::string_view msg);

public:
    ConnectionHandler();
    ~ConnectionHandler();

    void connect();
    void disconnect();
};
