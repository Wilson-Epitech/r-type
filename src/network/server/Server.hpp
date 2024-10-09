/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** Server
*/

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Lib.hpp"
#include <array>
#include <asio.hpp>
#include <map>
#include <memory>
#include <thread>

#define UPDATE_SERVER 0.02
#define MAXMESSAGE 255
#define MAXCLIENTS 4

namespace network {
    class Server {
      public:
        Server(asio::io_context &ioContext, std::string host, std::size_t port, asio::thread_pool &pool);
        ~Server();

        void sendToAClient(const std::size_t idClient, const std::string message, uint8_t messageType);
        void sendToAllClient(const std::string message, uint8_t messageType);
        std::map<std::size_t, asio::ip::udp::endpoint> getConnectedClients();
        std::size_t getNbClients();
        std::size_t getLastConnectedClientId();
        std::unordered_map<std::size_t, bool> getAreClientsWorldSet();
        DataToSend getEntityData();
        bool getDataHasBeenUsed();
        void setDataHasBeenUsed(bool value);

      private:
        void startReceiving(void);
        void deserializeMessage(const std::string message);
        void handleReceive(const std::error_code &error, std::size_t bytesTransferred);
        void handleSend(const std::error_code &error, std::size_t bytesTransferred);
        std::size_t logNewConnection(asio::ip::udp::endpoint endpoint);
        void keepAlive(std::size_t talkingClient);

        std::thread _ioThread;
        asio::ip::udp::socket _socket;
        asio::ip::udp::endpoint _remoteEndpoint;
        std::array<char, 100000> _receiveBuffer;

        std::size_t _idClient;
        std::map<std::size_t, asio::ip::udp::endpoint> _connectedClients;

        asio::thread_pool &_threadPool;

        bool _threadRunning;
        asio::thread_pool &_threadPoolHeartbeat;
        std::map<std::size_t, std::chrono::milliseconds> _clientHeartbeat;

        const uint16_t _magicNumber;
        uint16_t _receivedMagicNbr;
        uint8_t _messageType;
        uint8_t _messageId;
        uint16_t _messageLen;

        std::unordered_map<std::size_t, bool> _areClientsWorldSet;

        DataToSend _data;
        bool _dataHasBeenUsed;
    };
}
#endif
