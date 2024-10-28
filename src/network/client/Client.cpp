/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** Client
*/

#include "Client.hpp"
#include <asio.hpp>
#include <iostream>

network::Client::Client(const std::string &host, const std::string &port) : network::ANetwork(), _socket(_ioContext), _playerId(0) {
    _socket.open(asio::ip::udp::v4());
    asio::ip::udp::resolver resolver(_ioContext);
    _receiveEndpoint = *resolver.resolve(host, port).begin();
    _receiverThread = std::thread(&network::Client::msgFromServer, this);
    sendMessage("New client connected.", 1);
}

network::Client::~Client() {
    _isRunning = false;
    _receiverThread.join();
    _socket.close();
}

std::list<DataToSend> network::Client::getData() {
    return _data;
}

bool network::Client::isConnectedInNetwork() {
    return _isRunning;
}

network::Type network::Client::getNetworkType() {
    return network::Type::CLIENT;
}

void network::Client::msgFromServer() {
    std::array<char, 50000> receiveBuffer;
    asio::ip::udp::endpoint senderEndpoint;
    uint16_t receivedMagicNbr;
    uint8_t messageType;
    uint8_t messageId;
    uint16_t messageLen;

    while (_isRunning) {
        asio::error_code error;
        std::size_t len = _socket.receive_from(asio::buffer(receiveBuffer), senderEndpoint, 0, error);

        if (error) {
            std::cerr << "Receive failed: " << error.message() << std::endl;
            break;
        }

        if (len < sizeof(_magicNumber) + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint16_t)) {
            std::cerr << "Incomplete message received" << std::endl;
            continue;
        }

        std::memcpy(&receivedMagicNbr, receiveBuffer.data(), sizeof(receivedMagicNbr));

        if (receivedMagicNbr != _magicNumber) {
            std::cerr << "Invalid magic number" << std::endl;
            continue;
        }

        std::memcpy(&messageType, receiveBuffer.data() + sizeof(receivedMagicNbr), sizeof(messageType));
        std::memcpy(&messageId, receiveBuffer.data() + sizeof(receivedMagicNbr) + sizeof(messageType), sizeof(messageId));
        std::memcpy(&messageLen, receiveBuffer.data() + sizeof(receivedMagicNbr) + sizeof(messageType) + sizeof(messageId),
                    sizeof(messageLen));

        std::string receivedMessage(
            receiveBuffer.data() + sizeof(receivedMagicNbr) + sizeof(messageType) + sizeof(messageId) + sizeof(messageLen), messageLen);
        std::string newConnectionMessage = "New connection: ";
        std::string playerIdMessage = "playerId:";
        std::string disconnectedClient = "Disconnected:";

        if (receivedMessage.compare(0, playerIdMessage.length(), playerIdMessage) == 0) {
            _playerId = std::atoi(receivedMessage.substr(playerIdMessage.length()).c_str());
        } else if (receivedMessage.compare(0, newConnectionMessage.length(), newConnectionMessage) != 0 &&
                   receivedMessage.find(disconnectedClient) == std::string::npos && receivedMessage.length() > 0) {
            _data.push_back(network::HandleData::deserializeEntityDataVector(receivedMessage));
        }
        if (receivedMessage == "Could not connect") {
            _isRunning = false;
            std::cerr << "Server is full." << std::endl;
        }
    }
}

void network::Client::sendMessage(const std::string &message, uint8_t messageType) {
    _messageLen = static_cast<uint16_t>(message.size());
    std::vector<char> buffer(sizeof(_magicNumber) + sizeof(messageType) + sizeof(_messageId) + sizeof(_messageLen) + message.size());

    std::memcpy(buffer.data(), &_magicNumber, sizeof(_magicNumber));
    std::memcpy(buffer.data() + sizeof(_magicNumber), &messageType, sizeof(messageType));
    std::memcpy(buffer.data() + sizeof(_magicNumber) + sizeof(messageType), &_messageId, sizeof(_messageId));
    std::memcpy(buffer.data() + sizeof(_magicNumber) + sizeof(messageType) + sizeof(_messageId), &_messageLen, sizeof(_messageLen));
    std::memcpy(buffer.data() + sizeof(_magicNumber) + sizeof(messageType) + sizeof(_messageId) + sizeof(_messageLen), message.data(),
                message.size());

    if (_messageId >= MAXMESSAGE) {
        _messageId = 0;
    } else {
        _messageId += 1;
    }

    _socket.send_to(asio::buffer(buffer), _receiveEndpoint);
}

std::size_t network::Client::manageConnection() {
    static std::chrono::steady_clock::time_point time0 = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point time1 = std::chrono::steady_clock::now();
    std::chrono::milliseconds timeElapsed = std::chrono::duration_cast<std::chrono::seconds>(time1 - time0);
    if (!_isRunning)
        return 0;
    if (timeElapsed.count() > HEARTBEAT_TIME && _playerId != 0) {
        sendMessage("Alive: " + std::to_string(_playerId), 4);
        time0 = std::chrono::steady_clock::now();
    }
    return 0;
}

void network::Client::updateData(ecs::Registry &reg) {
    std::map<std::size_t, entity_t> player = reg.getPlayers();
    if (_playerId != 0 && player.size() == 0)
        reg.addPlayer(0, _playerId);
    for (auto dataReceived : _data) {
        if (!dataReceived.empty()) {
            network::HandleData::updateDeadEntities(dataReceived, reg);
            network::HandleData::updateEntitiesData(dataReceived, reg);
        }
    }
    _data.clear();
}

void network::Client::sendUpdatedData(ecs::Registry &reg, std::list<DataToSend> &lastDataReceivedList) {
    DataToSend data = network::HandleData::fillNewEntitiesDataToSend(reg);

    for (auto lastDataReceived : lastDataReceivedList)
        network::HandleData::deleteSimilarDataLines(data, lastDataReceived);
    if (!data.empty()) {
        std::string serialized = network::HandleData::serializeEntityDataVector(data);
        sendMessage(serialized, 3);
    }
}
