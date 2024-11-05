#include "connectionHandler.h"
#include "messages.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <boost/asio/ssl.hpp>

namespace
{
    auto getSubscribeMessage(std::string_view product_id)
    {
        return SubscribeMessage{
            .type = "subscribe",
            .product_ids = std::vector<std::string>{std::string(product_id)},
            .channels = {"ticker"}};
    }
}

ConnectionHandler::ConnectionHandler()
    : statsTracker{Config::EMA_WINDOW, Config::CSV_FILENAME}
{
    client.set_access_channels(websocketpp::log::alevel::none);
    client.set_error_channels(websocketpp::log::elevel::fatal);
    client.init_asio();
    setupHandlers();
}

ConnectionHandler::~ConnectionHandler()
{
    disconnect();
}

void ConnectionHandler::setupHandlers()
{
    client.set_message_handler([this](ConnectionPtr conn, Client::message_ptr msg)
                               { onMessage(msg->get_payload()); });

    client.set_open_handler([this](ConnectionPtr conn)
                            {
        std::cout << "Connection established" << std::endl;
        subscribe(); });

    client.set_close_handler([this](ConnectionPtr conn)
                             { std::cout << "Connection closed" << std::endl; });

    client.set_tls_init_handler([](ConnectionPtr conn)
                                { return websocketpp::lib::make_shared<boost::asio::ssl::context>(
                                      boost::asio::ssl::context::tlsv12_client); });
}

void ConnectionHandler::connect()
{
    try
    {
        websocketpp::lib::error_code ec;
        auto conn = client.get_connection(std::string(Config::WEBSOCKET_URI), ec);
        if (ec)
        {
            std::cerr << "Connect initialization error: " << ec.message() << std::endl;
            return;
        }

        connection = conn->get_handle();
        client.connect(conn);
        client.run();
    }
    catch (const websocketpp::exception &e)
    {
        std::cerr << "Connect error: " << e.what() << std::endl;
    }
}

void ConnectionHandler::disconnect()
{
    client.close(connection, websocketpp::close::status::normal, "Closing connection");
}

void ConnectionHandler::sendMessage(std::string_view msg)
{
    try
    {
        auto j = nlohmann::json::parse(msg);
        client.send(connection, std::string(msg), websocketpp::frame::opcode::text);
    }
    catch (const nlohmann::json::parse_error &e)
    {
        std::cerr << "Invalid JSON: " << e.what() << std::endl;
    }
    catch (const websocketpp::exception &e)
    {
        std::cerr << "Error sending message: " << e.what() << std::endl;
    }
}

std::optional<std::string> ConnectionHandler::getMsgType(const nlohmann::json &j)
{
    if (!j.contains("type"))
    {
        return std::nullopt;
    }
    return j["type"].get<std::string>();
}

void ConnectionHandler::subscribe()
{
    auto msg = getSubscribeMessage(Config::PRODUCT_ID);
    nlohmann::json j = msg;
    sendMessage(j.dump());
}

void ConnectionHandler::onSubscriptionSuccess()
{
    std::cout << "Subscription successful" << std::endl;
}

void ConnectionHandler::onMessage(std::string msg)
{
    try
    {
        auto j = nlohmann::json::parse(msg);
        auto msgType = getMsgType(j);
        if (!msgType)
        {
            std::cout << "No message type found" << std::endl;
            return;
        }

        if (*msgType == "ticker")
        {
            auto tickerWire = j.get<TickerWire>();
            auto ticker = Ticker::fromWire(tickerWire);
            statsTracker.onTickerReceived(ticker);
        }
        else if (*msgType == "subscriptions")
        {
            onSubscriptionSuccess();
        }
        else
        {
            std::cerr << "Unknown message type: " << *msgType << std::endl;
        }
    }
    catch (const nlohmann::json::parse_error &e)
    {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
    }
}
