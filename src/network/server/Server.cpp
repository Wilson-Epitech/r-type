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
    : _socket(io_context, asio::ip::udp::endpoint(asio::ip::make_address(host), port)), _idClient(0), _threadPool(pool),
      _threadRunning(false), _threadPoolHeartbeat(pool), _magicNumber(0x9B3D), _dataHasBeenUsed(false) {
    startReceiving();
    _ioThread = std::thread([&io_context]() { io_context.run(); });
    for (std::size_t i = 0; i < MAXCLIENTS; i++)
        _areClientsWorldSet[i] = false;
}

network::Server::~Server() {
    _threadPoolHeartbeat.join();
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
        std::memcpy(&_receivedMagicNbr, _receiveBuffer.data(), sizeof(_receivedMagicNbr));

        if (_receivedMagicNbr != _magicNumber) {
            std::cerr << "Invalid magic number" << std::endl;
            startReceiving();
            return;
        }

        std::memcpy(&_messageType, _receiveBuffer.data() + sizeof(_receivedMagicNbr), sizeof(_messageType));
        std::memcpy(&_messageId, _receiveBuffer.data() + sizeof(_receivedMagicNbr) + sizeof(_messageType), sizeof(_messageId));
        std::memcpy(&_messageLen, _receiveBuffer.data() + sizeof(_receivedMagicNbr) + sizeof(_messageType) + sizeof(_messageId),
                    sizeof(_messageLen));

        if (byteTransfered != sizeof(_receivedMagicNbr) + sizeof(_messageType) + sizeof(_messageId) + sizeof(_messageLen) + _messageLen) {
            std::cerr << "Incomplete message received" << std::endl;
            startReceiving();
            return;
        }

        std::string message(_receiveBuffer.data() + sizeof(_receivedMagicNbr) + sizeof(_messageType) + sizeof(_messageId) +
                                sizeof(_messageLen),
                            _messageLen);

        std::size_t talkingClient = logNewConnection(_remoteEndpoint);
        if (talkingClient == 0) {
            startReceiving();
            return;
        }
        if (_threadRunning == false)
            asio::post(_threadPoolHeartbeat, std::bind(&network::Server::keepAlive, this, talkingClient));

        _threadRunning = true;

        if (message == "Client disconnected") {
            _clientHeartbeat.erase(talkingClient);
            _connectedClients.erase(talkingClient);
            std::string msg = "Client disconnected: " + std::to_string(talkingClient);
            sendToAllClient(msg, 2);
        }

        std::string newClientConnectedMessage = "New client connected.";
        std::string aliveMessage = "Alive:";
        std::string worldSetMessage = "World set";

        if (message.find(newClientConnectedMessage) == std::string::npos && message.find(aliveMessage) == std::string::npos &&
            message.length() > 0) {
            if (message.compare(0, worldSetMessage.length(), worldSetMessage) == 0) {
                _areClientsWorldSet[talkingClient - 1] = true;
            } else {
                std::cout << "data:" << message << std::endl;
                deserializeMessage(message);
            }
        }
        startReceiving();
        _receiveBuffer.fill(0);
    } else {
        std::cerr << "Receive error: " << error.message() << std::endl;
    }
}

void network::Server::handleSend(const std::error_code &error, std::size_t) {
    if (error)
        std::cerr << "Failed to send message. Error: " << error.message() << std::endl;
}

void network::Server::sendToAClient(const std::size_t idClient, const std::string messageToSend, uint8_t type) {
    auto it = _connectedClients.find(idClient);
    uint16_t len = static_cast<uint16_t>(messageToSend.size());
    std::string buffer(sizeof(_magicNumber) + sizeof(type) + sizeof(_messageId) + sizeof(len) + len, '\0');

    _messageId += 1;
    if (_messageId >= MAXMESSAGE) {
        _messageId = 0;
    }

    std::memcpy(&buffer[0], &_magicNumber, sizeof(_magicNumber));
    std::memcpy(&buffer[sizeof(_magicNumber)], &type, sizeof(type));
    std::memcpy(&buffer[sizeof(_magicNumber) + sizeof(type)], &_messageId, sizeof(_messageId));
    std::memcpy(&buffer[sizeof(_magicNumber) + sizeof(type) + sizeof(_messageId)], &len, sizeof(len));
    std::memcpy(&buffer[sizeof(_magicNumber) + sizeof(type) + sizeof(_messageId) + sizeof(len)], messageToSend.data(),
                messageToSend.size());

    if (it != _connectedClients.end()) {
        std::shared_ptr<std::string> message(new std::string(buffer));

        _socket.async_send_to(asio::buffer(*message), it->second,
                              std::bind(&network::Server::handleSend, this, std::placeholders::_1, std::placeholders::_2));
    } else {
        std::cerr << "Client " << idClient << "not found" << std::endl;
    }
}

void network::Server::sendToAllClient(const std::string messageToSend, uint8_t messageType) {
    uint16_t len = static_cast<uint16_t>(messageToSend.size());
    std::string buffer(sizeof(_magicNumber) + sizeof(messageType) + sizeof(_messageId) + sizeof(len) + len, '\0');

    _messageId += 1;
    if (_messageId >= MAXMESSAGE) {
        _messageId = 0;
    }

    std::memcpy(&buffer[0], &_magicNumber, sizeof(_magicNumber));
    std::memcpy(&buffer[sizeof(_magicNumber)], &messageType, sizeof(messageType));
    std::memcpy(&buffer[sizeof(_magicNumber) + sizeof(messageType)], &_messageId, sizeof(_messageId));
    std::memcpy(&buffer[sizeof(_magicNumber) + sizeof(messageType) + sizeof(_messageId)], &len, sizeof(len));
    std::memcpy(&buffer[sizeof(_magicNumber) + sizeof(messageType) + sizeof(_messageId) + sizeof(len)], messageToSend.data(),
                messageToSend.size());

    for (auto &it : _connectedClients) {
        std::shared_ptr<std::string> message(new std::string(buffer));

        _socket.async_send_to(asio::buffer(*message), it.second,
                              std::bind(&network::Server::handleSend, this, std::placeholders::_1, std::placeholders::_2));
    }
}

std::size_t network::Server::logNewConnection(asio::ip::udp::endpoint endpoint) {
    for (const auto &it : _connectedClients) {
        if (it.second == endpoint)
            return it.first;
    }

    if (_connectedClients.size() >= MAXCLIENTS) {
        std::string messageToSend = "Could not connect";
        uint8_t messageType = 5;
        uint16_t len = static_cast<uint16_t>(messageToSend.size());
        std::string buffer(sizeof(_magicNumber) + sizeof(messageType) + sizeof(_messageId) + sizeof(len) + len, '\0');

        _messageId += 1;
        if (_messageId >= MAXMESSAGE) {
            _messageId = 0;
        }

        std::memcpy(&buffer[0], &_magicNumber, sizeof(_magicNumber));
        std::memcpy(&buffer[sizeof(_magicNumber)], &messageType, sizeof(messageType));
        std::memcpy(&buffer[sizeof(_magicNumber) + sizeof(messageType)], &_messageId, sizeof(_messageId));
        std::memcpy(&buffer[sizeof(_magicNumber) + sizeof(messageType) + sizeof(_messageId)], &len, sizeof(len));
        std::memcpy(&buffer[sizeof(_magicNumber) + sizeof(messageType) + sizeof(_messageId) + sizeof(len)], messageToSend.data(),
                    messageToSend.size());
        std::cerr << "Server is full." << std::endl;
        std::shared_ptr<std::string> message(new std::string(buffer));

        _socket.async_send_to(asio::buffer(*message), _remoteEndpoint,
                              std::bind(&network::Server::handleSend, this, std::placeholders::_1, std::placeholders::_2));
        return 0;
    }

    _idClient++;
    _connectedClients[_idClient] = endpoint;
    sendToAllClient("New connection: " + std::to_string(_idClient), 1);
    _threadRunning = false;

    asio::post(_threadPool, std::bind(&network::Server::startReceiving, this));

    return _idClient;
}

void network::Server::keepAlive(std::size_t talkingClient) {
    std::chrono::steady_clock::time_point time0 = std::chrono::steady_clock::now();
    while (1) {
        std::chrono::steady_clock::time_point time1 = std::chrono::steady_clock::now();
        std::chrono::milliseconds timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(time1 - time0);
        _clientHeartbeat[talkingClient] = timeElapsed;

        std::memcpy(&_receivedMagicNbr, _receiveBuffer.data(), sizeof(_receivedMagicNbr));

        std::string checkSize = _receiveBuffer.data();

        if (checkSize.length() > 0 && _receivedMagicNbr != _magicNumber) {
            std::cerr << "KeepAlive:Invalid magic number" << std::endl;
            continue;
        }

        std::memcpy(&_messageType, _receiveBuffer.data() + sizeof(_receivedMagicNbr), sizeof(_messageType));
        std::memcpy(&_messageId, _receiveBuffer.data() + sizeof(_receivedMagicNbr) + sizeof(_messageType), sizeof(_messageId));
        std::memcpy(&_messageLen, _receiveBuffer.data() + sizeof(_receivedMagicNbr) + sizeof(_messageType) + sizeof(_messageId),
                    sizeof(_messageLen));

        std::string message(_receiveBuffer.data() + sizeof(_receivedMagicNbr) + sizeof(_messageType) + sizeof(_messageId) +
                                sizeof(_messageLen),
                            _messageLen);
        std::string aliveMsg = "Alive: ";

        if (message.find(aliveMsg, 0) != std::string::npos && (std::size_t)std::stoi(message.substr(aliveMsg.size())) == talkingClient) {
            time0 = std::chrono::steady_clock::now();
        }
        if (_clientHeartbeat[talkingClient] >= std::chrono::seconds(5)) {
            std::string message = "Client disconnected: " + std::to_string(talkingClient);
            _connectedClients.erase(talkingClient);
            _clientHeartbeat.erase(talkingClient);
            sendToAllClient(message, 2);
            break;
        }
    }
}
