/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** Server
*/

/**
 * @file Server.hpp
 * @brief Declares the Server class that handles client-server communication in the R-Type game.
 * The Server class manages client connections, processes incoming messages, and sends responses.
 * It uses UDP protocol for communication and handles operations asynchronously.
 */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "ANetwork.hpp"
#include <map>
#include <memory>

#define NB_CLIENTS_AUTHORIZED 4
#define MAXMESSAGE 255
#define CHECK_HEARTBEAT_TIME 5

namespace network {

    /**
     * @class Server
     * @brief Handles client connections and message exchanges for the R-Type game server.
     * The server listens for incoming client connections, manages multiple clients,
     * and handles message sending/receiving operations using the asio library.
     */
    class Server : virtual public network::ANetwork {
      public:
        Server(const std::string &host, const std::size_t &port, asio::thread_pool &pool);
        ~Server();

        void sendMessage(const std::string &message, uint8_t messageType) override;
        void sendUpdatedData(ecs::Registry &reg, std::list<DataToSend> &lastDataReceivedList) override;
        std::size_t manageConnection() override;
        void updateData(ecs::Registry &reg) override;
        std::list<DataToSend> getData() override;
        network::Type getNetworkType() override;
        bool isConnectedInNetwork() override;

      private:

        void startReceiving();
        void sendToAClient(const std::size_t idClient, const std::string message, uint8_t messageType);
        void sendToAllClient(const std::string message, uint8_t messageType);
        void handleReceive(const std::error_code &error, std::size_t bytesTransferred);
        void handleSend(const std::error_code &error, std::size_t bytesTransferred);
        std::size_t logNewConnection(asio::ip::udp::endpoint endpoint);
        void setHeartBeatMap(std::unordered_map<std::size_t, std::chrono::steady_clock::time_point>);
        void setClientStillHere(std::unordered_map<std::size_t, bool>);
        std::size_t getLastConnectedClientId();
        std::size_t getStillAliveClientNumber();
        std::unordered_map<std::size_t, bool> getClientStillHere();
        std::unordered_map<std::size_t, bool> getAreClientsWorldSet();
        std::unordered_map<std::size_t, std::size_t> getCorrespondence();
        std::unordered_map<std::size_t, std::chrono::steady_clock::time_point> getHeartbeatClient();

        asio::io_context _ioContext;
        std::thread _ioThread;                   ///< The thread running the asio context.
        asio::ip::udp::socket _socket;           ///< The UDP socket used for communication.
        asio::ip::udp::endpoint _remoteEndpoint; ///< Endpoint of the client sending data.
        std::array<char, 50000> _receiveBuffer;  ///< Buffer for storing received data.

        std::size_t _idClient;          ///< The ID of the last connected client.
        asio::thread_pool &_threadPool; ///< Thread pool for handling tasks.

        std::map<std::size_t, asio::ip::udp::endpoint> _connectedClients; ///< Map of connected clients and their endpoints.
        std::unordered_map<std::size_t, bool> _areClientsWorldSet;        ///< Map indicating whether each client's world is initialized.

        std::unordered_map<std::size_t, std::chrono::steady_clock::time_point> _lastAliveClient; ///< Heartbeat timestamps for clients.
        std::unordered_map<std::size_t, bool>
            _isClientStillHere; ///<  Map that set to false the bool if the client with the std::size_t id was disconnected
        std::unordered_map<std::size_t, std::size_t> _corresOwnPlayerId; ///< Create a coresspondence between the _OwnID and the _playerID;

        uint16_t _receivedMagicNbr;  ///< Received magic number for message validation.
        uint8_t _messageType;        ///< Type of the received message.
    };
};

extern "C"
{
    network::Server* entryPoint(const std::string &host, const std::size_t &port, asio::thread_pool &pool) {
        return new network::Server(host, port, pool);
    }
}

#endif
