/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** Client
*/

#include "Client.hpp"
#include <asio.hpp>
#include <iostream>

network::Client::Client(asio::io_context &ioContext, const std::string &host, const std::string &port) : _socket(ioContext), _isRunning(true), _magicNumber(0x9B3D), _messageId(0), _playerId(0) {
    _socket.open(asio::ip::udp::v4());
    asio::ip::udp::resolver resolver(ioContext);
    _receiveEndpoint = *resolver.resolve(host, port).begin();
    _receiverThread = std::thread(&network::Client::msgFromServer, this);
    _heartbeatThread = std::thread(&network::Client::heartbeatToServer, this);
    sendMessage("New client connected.", 1);
}

network::Client::~Client() {
    _isRunning = false;
    _receiverThread.join();
    _heartbeatThread.join();
    _socket.close();
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
        std::memcpy(&messageLen, receiveBuffer.data() + sizeof(receivedMagicNbr) + sizeof(messageType) + sizeof(messageId), sizeof(messageLen));

        std::string receivedMessage(receiveBuffer.data() + sizeof(receivedMagicNbr) + sizeof(messageType) + sizeof(messageId) + sizeof(messageLen), messageLen);
        std::string newConnectionMessage = "New connection: ";
        std::string playerIdMessage = "playerId:";
        std::string createWorldMessage = "Create World:";
        std::string disconnectedClient = "Disconnected:";

        if (receivedMessage.compare(0, playerIdMessage.length(), playerIdMessage) == 0) {
            _playerId = std::atoi(receivedMessage.substr(playerIdMessage.length()).c_str());
        } else if (receivedMessage.compare(0, newConnectionMessage.length(), newConnectionMessage) != 0
                    && receivedMessage.find(disconnectedClient) == std::string::npos && receivedMessage.length() > 0) {
            _data = Lib::deserializeEntityDataVector(receivedMessage);
        }
        if (receivedMessage == "Could not connect") {
            _isRunning = false;
            std::cerr << "Server is full." << std::endl;
        }
    }
}

void network::Client::sendMessage(const std::string& message, uint8_t messageType) {
    _len = static_cast<uint16_t>(message.size());
    std::vector<char> buffer(sizeof(_magicNumber) + sizeof(messageType) + sizeof(_messageId) + sizeof(_len) + message.size());

    std::memcpy(buffer.data(), &_magicNumber, sizeof(_magicNumber));
    std::memcpy(buffer.data() + sizeof(_magicNumber), &messageType, sizeof(messageType));
    std::memcpy(buffer.data() + sizeof(_magicNumber) + sizeof(messageType), &_messageId, sizeof(_messageId));
    std::memcpy(buffer.data() + sizeof(_magicNumber) + sizeof(messageType) + sizeof(_messageId), &_len, sizeof(_len));
    std::memcpy(buffer.data() + sizeof(_magicNumber) + sizeof(messageType) + sizeof(_messageId) + sizeof(_len), message.data(), message.size());

    if (_messageId >= MAXMESSAGE) {
        _messageId = 0;
    } else {
        _messageId += 1;
    }

    _socket.send_to(asio::buffer(buffer), _receiveEndpoint);
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
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (_playerId != 0)
            sendMessage("Alive: " + std::to_string(_playerId), 4);
    }
}
