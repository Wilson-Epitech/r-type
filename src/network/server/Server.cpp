/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** server
*/

#include "Server.hpp"
#include <iostream>
#include <functional>

Server::Server(asio::io_context& io_context, std::size_t port) : _socket(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)), _idClient(0)
{
    startReceiving();
}

void Server::startReceiving()
{
    _socket.async_receive_from(
        asio::buffer(_receiveBuffer), _remoteEndpoint,
        std::bind(&Server::handleReceive, this,
        std::placeholders::_1,
        std::placeholders::_2));
}

void Server::handleReceive(const std::error_code& error, std::size_t byteTransfered)
{
    if (!error) {
        std::size_t talkingClient = logNewConnection(_remoteEndpoint);
        if (talkingClient == 0) {
            startReceiving();
            return;
        }
        std::cout << "Server received msg from client " << talkingClient << ": " << std::string(_receiveBuffer.data(), byteTransfered) << std::endl;
        sendToAClient(1, "TEST ONE CLIENT");
        sendToAllClient("TEST ALL CLIENT");

        std::shared_ptr<std::string> message(new std::string("Server received"));

        _socket.async_send_to(asio::buffer(*message), _remoteEndpoint,
            std::bind(&Server::handleSend, this, message,
                std::placeholders::_1,
                std::placeholders::_2));

        startReceiving();
        _receiveBuffer.fill(0);
    } else {
        std::cerr << "Receive error: " << error.message() << std::endl;
    }
}

void Server::handleSend(std::shared_ptr<std::string> message, const std::error_code& error, std::size_t)
{
    if (error)
        std::cerr << "Failed to send message. Error: " << error.message() << std::endl;
}

void Server::sendToAClient(const std::size_t idClient, const std::string messageToSend)
{
    auto it = _connectedClients.find(idClient);

    if (it != _connectedClients.end()) {
        std::shared_ptr<std::string> message(new std::string (messageToSend));

        _socket.async_send_to(asio::buffer(*message), it->second,
            std::bind(&Server::handleSend, this, message,
                std::placeholders::_1,
                std::placeholders::_2));
    } else {
        std::cerr << "Client " << idClient << "not found" << std::endl;
    }
}

void Server::sendToAllClient(const std::string messageToSend)
{
    for (auto &it: _connectedClients) {
        std::shared_ptr<std::string> message(new std::string (messageToSend));

        _socket.async_send_to(asio::buffer(*message), it.second,
            std::bind(&Server::handleSend, this, message,
                std::placeholders::_1,
                std::placeholders::_2));
    }
}

std::size_t Server::logNewConnection(asio::ip::udp::endpoint endpoint)
{
    for (const auto& it: _connectedClients) {
        if (it.second == endpoint)
            return it.first;
    }

    if (_connectedClients.size() >= 4) {
        std::cerr << "Server is full." << std::endl;
        return 0;
    }

    _idClient++;
    _connectedClients[_idClient] = endpoint;
    std::cout << "New client connected: " << _idClient << std::endl;
    return _idClient;

}
