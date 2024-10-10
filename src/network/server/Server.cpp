/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** server
*/

#include "Server.hpp"
#include <functional>
#include <iostream>

network::Server::Server(asio::io_context &io_context, std::string host, std::size_t port, asio::thread_pool &pool)
    : _socket(io_context, asio::ip::udp::endpoint(asio::ip::make_address(host), port)), _idClient(0), _threadPool(pool), _dataHasBeenUsed(false) {
    startReceiving();
    _ioThread = std::thread([&io_context]() { io_context.run(); });
    for (std::size_t i = 0; i < NB_CLIENTS_AUTHORIZED; i++)
        _areClientsWorldSet[i] = false;
}

network::Server::~Server() {
    _ioThread.join();
}

std::map<std::size_t, asio::ip::udp::endpoint> network::Server::getConnectedClients() {
    return _connectedClients;
}

std::size_t network::Server::getNbClients() {
    return _connectedClients.size();
}

std::size_t network::Server::getLastConnectedClientId() {
    return _idClient;
}

std::unordered_map<std::size_t, bool> network::Server::getAreClientsWorldSet() {
    return _areClientsWorldSet;
}

DataToSend network::Server::getEntityData() {
    return _data;
}

bool network::Server::getDataHasBeenUsed() {
    return _dataHasBeenUsed;
}

void network::Server::setDataHasBeenUsed(bool value) {
    _dataHasBeenUsed = value;
}

std::unordered_map<std::size_t, std::chrono::steady_clock::time_point> network::Server::getHeartbeatClient() {
    return _lastAliveClient;
}

void network::Server::setHeartBeatmMap(std::unordered_map<std::size_t, std::chrono::steady_clock::time_point> map)
{
    _lastAliveClient = map;
}

void network::Server::startReceiving() {
    _socket.async_receive_from(asio::buffer(_receiveBuffer), _remoteEndpoint,
                               std::bind(&network::Server::handleReceive, this, std::placeholders::_1, std::placeholders::_2));
}

void network::Server::deserializeMessage(const std::string message) {
    _data.deadEntities.clear();
    _data.entitiesData.clear();
    _data = Lib::deserializeEntityDataVector(message);
    _dataHasBeenUsed = false;
}

void network::Server::handleReceive(const std::error_code &error, std::size_t byteTransfered) {
    if (!error) {
        std::size_t talkingClient = logNewConnection(_remoteEndpoint);
        if (talkingClient == 0) {
            startReceiving();
            return;
        }
        std::string receivedMessage = std::string(_receiveBuffer.data(), byteTransfered);
        std::string newClientConnectedMessage = "New client connected.";
        std::string worldSetMessage = "World set";
        std::string aliveMessage = "Alive: ";

        if (receivedMessage.compare(0, worldSetMessage.length(), worldSetMessage) == 0) {
            _areClientsWorldSet[talkingClient - 1] = true;
        } else if (receivedMessage.compare(0, aliveMessage.length(), aliveMessage) == 0) {
            std::size_t clientId = std::stoi(receivedMessage.substr(aliveMessage.length()));
            _lastAliveClient[clientId] = std::chrono::steady_clock::now();
            std::cerr << "Received heartbeat from client " << clientId << std::endl;
        } else if (receivedMessage.compare(0, newClientConnectedMessage.length(), newClientConnectedMessage) != 0 &&
                   receivedMessage.length() > 0) {
            std::cout << "data:" << receivedMessage << std::endl;
            deserializeMessage(receivedMessage);
        }
        startReceiving();
        _receiveBuffer.fill(0);
    } else {
        std::cerr << "Receive error: " << error.message() << std::endl;
    }
}

void network::Server::handleSend(std::shared_ptr<std::string> message, const std::error_code &error, std::size_t) {
    if (error)
        std::cerr << "Failed to send message. Error: " << error.message() << std::endl;
}

void network::Server::sendToAClient(const std::size_t idClient, const std::string messageToSend) {
    auto it = _connectedClients.find(idClient);

    if (it != _connectedClients.end()) {
        std::shared_ptr<std::string> message(new std::string(messageToSend));

        _socket.async_send_to(asio::buffer(*message), it->second,
                              std::bind(&network::Server::handleSend, this, message, std::placeholders::_1, std::placeholders::_2));
    } else {
        std::cerr << "Client " << idClient << "not found" << std::endl;
    }
}

void network::Server::sendToAllClient(const std::string messageToSend) {
    for (auto &it : _connectedClients) {
        std::shared_ptr<std::string> message(new std::string(messageToSend));

        _socket.async_send_to(asio::buffer(*message), it.second,
                              std::bind(&network::Server::handleSend, this, message, std::placeholders::_1, std::placeholders::_2));
    }
}

std::size_t network::Server::logNewConnection(asio::ip::udp::endpoint endpoint) {
    for (const auto &it : _connectedClients) {
        if (it.second == endpoint)
            return it.first;
    }

    if (_connectedClients.size() >= 4) {
        std::cerr << "Server is full." << std::endl;
        std::shared_ptr<std::string> message(new std::string("Could not connect"));

        _socket.async_send_to(asio::buffer(*message), _remoteEndpoint,
                              std::bind(&network::Server::handleSend, this, message, std::placeholders::_1, std::placeholders::_2));
        return 0;
    }

    _idClient++;
    _connectedClients[_idClient] = endpoint;
    sendToAllClient("New connection: " + std::to_string(_idClient));

    asio::post(_threadPool, std::bind(&network::Server::startReceiving, this));

    return _idClient;
}
