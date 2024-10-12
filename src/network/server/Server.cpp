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
      _dataHasBeenUsed(false), _magicNumber(0x9B3D) {
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

void network::Server::setHeartBeatmMap(std::unordered_map<std::size_t, std::chrono::steady_clock::time_point> map) {
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

        if (message == "Client disconnected") {
            _connectedClients.erase(talkingClient);
            std::string msg = "Client disconnected: " + std::to_string(talkingClient);
            sendToAllClient(msg, 2);
        }

        std::string newClientConnectedMessage = "New client connected.";
        std::string worldSetMessage = "World set";
        std::string aliveMessage = "Alive: ";

        if (message.compare(0, worldSetMessage.length(), worldSetMessage) == 0) {
            _areClientsWorldSet[talkingClient - 1] = true;
        } else if (message.compare(0, aliveMessage.length(), aliveMessage) == 0) {
            std::size_t clientId = std::stoi(message.substr(aliveMessage.length()));
            _lastAliveClient[clientId] = std::chrono::steady_clock::now();
            std::cerr << "Received heartbeat from client " << clientId << std::endl;
        } else if (message.compare(0, newClientConnectedMessage.length(), newClientConnectedMessage) != 0 && message.length() > 0) {
            std::cout << "data:" << message << std::endl;
            deserializeMessage(message);
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

    if (_connectedClients.size() >= NB_CLIENTS_AUTHORIZED) {
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

    asio::post(_threadPool, std::bind(&network::Server::startReceiving, this));

    return _idClient;
}

std::size_t network::Server::checkConnectedHeartbeat() {
    std::unordered_map<std::size_t, std::chrono::steady_clock::time_point> lastSeenClient = getHeartbeatClient();
    auto now = std::chrono::steady_clock::now();
    auto it = lastSeenClient.begin();
    std::size_t disconnectedPlayerEntity = 0;

    while (it != lastSeenClient.end()) {
        if (std::chrono::duration_cast<std::chrono::seconds>(now - it->second).count() > 5) {
            sendToAllClient("Disconnected: " + std::to_string(it->first), 4);
            disconnectedPlayerEntity = it->first;
            it = lastSeenClient.erase(it);
        } else {
            it++;
        }
    }
    setHeartBeatmMap(lastSeenClient);
    return disconnectedPlayerEntity;
}

void network::Server::sendUpdatedData(ecs::Registry &reg) {

    std::map<std::size_t, asio::ip::udp::endpoint> connectedClients = getConnectedClients();

    static std::size_t previousConnectedClientsSize;
    std::size_t connectedClientsSize = connectedClients.size();

    static std::size_t previousLastConnectedClientId;
    std::size_t lastConnectedClientId = getLastConnectedClientId();
    std::unordered_map<std::size_t, bool> areClientsWorldSet = getAreClientsWorldSet();

    for (auto it = connectedClients.begin(); it != connectedClients.end(); it++) {
        std::size_t clientId = it->first;
        if (((connectedClientsSize > previousConnectedClientsSize) && (lastConnectedClientId == clientId) &&
             lastConnectedClientId != previousLastConnectedClientId) &&
            (lastConnectedClientId == clientId)) {
            entity_t newPlayer = reg.spawnEntity();
            reg.addPlayer(lastConnectedClientId, newPlayer);
            std::size_t playerId = reg.getPlayerByClientId(lastConnectedClientId);
            reg.runInitPlayerSystem();
            DataToSend allEntitiesData = Lib::fillAllEntitiesDataToSend(reg);
            std::string allEntititiesSerialized = Lib::serializeEntityDataVector(allEntitiesData);
            previousConnectedClientsSize = connectedClientsSize;
            previousLastConnectedClientId = lastConnectedClientId;
            sendToAClient(clientId, "playerId:" + std::to_string(playerId), 3);
            if (!allEntitiesData.empty())
                sendToAClient(clientId, allEntititiesSerialized, 3);
        }
    }

    DataToSend newEntitiesData = Lib::fillNewEntitiesDataToSend(reg);
    for (auto it = connectedClients.begin(); it != connectedClients.end(); it++) {
        std::size_t clientId = it->first;
        if (areClientsWorldSet[clientId - 1] && !newEntitiesData.empty()) {
            std::string newEntititiesSerialized = Lib::serializeEntityDataVector(newEntitiesData);
            sendToAClient(clientId, newEntititiesSerialized, 3);
        }
    }
}
