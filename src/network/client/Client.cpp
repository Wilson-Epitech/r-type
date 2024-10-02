/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** Client
*/

#include "Client.hpp"
#include <asio.hpp>
#include <iostream>

Client::Client(asio::io_context &ioContext, const std::string &host, const std::string &port) : _socket(ioContext), _isRunning(true) {
    _socket.open(asio::ip::udp::v4());
    asio::ip::udp::resolver resolver(ioContext);
    _receiveEndpoint = *resolver.resolve(asio::ip::udp::v4(), host, port).begin();
    _receiverThread = std::thread(&Client::msgFromServer, this);
    sendMessage("New client connected.");
}

Client::~Client() {
    _isRunning = false;
    _receiverThread.join();
    _socket.close();
}

void Client::msgFromServer() {
    std::array<char, 1024> receiveBuffer;
    asio::ip::udp::endpoint senderEndpoint;

    while (_isRunning) {
        asio::error_code error;
        std::size_t len = _socket.receive_from(asio::buffer(receiveBuffer), senderEndpoint, 0, error);

        if (error) {
            std::cerr << "Receive failed: " << error.message() << std::endl;
            break;
        }

        std::cout << "Server: " << std::string(receiveBuffer.data(), len) << std::endl;
        if (std::string(receiveBuffer.data(), len) == "Could not connect")
            _isRunning = false;
    }
}

void Client::sendMessage(const std::string &message) {
    _socket.send_to(asio::buffer(message), _receiveEndpoint);
}

bool Client::getIsRunning() {
    return _isRunning;
}
