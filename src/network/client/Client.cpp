/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** Client
*/

#include "Client.hpp"
#include <asio.hpp>
#include <iostream>

network::Client::Client(asio::io_context &ioContext, const std::string &host, const std::string &port) : _socket(ioContext), _isRunning(true) {
    _socket.open(asio::ip::udp::v4());
    asio::ip::udp::resolver resolver(ioContext);
    _receiveEndpoint = *resolver.resolve(host, port).begin();
    _receiverThread = std::thread(&network::Client::msgFromServer, this);
    _heartbeatThread = std::thread(&network::Client::heartbeatToServer, this);
    sendMessage("New client connected.");
}

network::Client::~Client() {
    _isRunning = false;
    _heartbeatThread.join();
    _receiverThread.join();
    _socket.close();
}

void network::Client::msgFromServer() {
    std::array<char, 50000> receiveBuffer;
    asio::ip::udp::endpoint senderEndpoint;

    while (_isRunning) {
        asio::error_code error;
        std::size_t len = _socket.receive_from(asio::buffer(receiveBuffer), senderEndpoint, 0, error);

        if (error) {
            std::cerr << "Receive failed: " << error.message() << std::endl;
            break;
        }
        std::string receivedMessage = std::string(receiveBuffer.data(), len);
        std::string newConnectionMessage = "New connection: ";
        std::string playerIdMessage = "playerId:";
        std::string createWorldMessage = "Create World:";
        std::string disconnectedClient = "Disconnected:";

        if (receivedMessage.compare(0, playerIdMessage.length(), playerIdMessage) == 0) {
            _playerId = std::atoi(receivedMessage.substr(playerIdMessage.length()).c_str());
            std::cout << _playerId << std::endl;
        } else if (receivedMessage.find(disconnectedClient) != std::string::npos) {
            std::cout << "CLIENT DISOCNNECTED" << std::endl;
        } else if (receivedMessage.compare(0, newConnectionMessage.length(), newConnectionMessage) != 0 && receivedMessage.length() > 0) {
            _data = Lib::deserializeEntityDataVector(receivedMessage);
            std::cout << "data:" << receivedMessage << std::endl;
        }
        if (receivedMessage == "Could not connect")
            _isRunning = false;
    }
}

void network::Client::sendMessage(const std::string &message) {
    _socket.send_to(asio::buffer(message), _receiveEndpoint);
}

bool network::Client::getIsRunning() {
    return _isRunning;
}

DataToSend network::Client::getData() {
    return _data;
}

std::size_t network::Client::getPlayerId() {
    return _playerId;
}

void network::Client::heartbeatToServer() {
    while (_isRunning) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        sendMessage("Alive: " + std::to_string(_playerId));
    }
}
