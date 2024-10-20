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

#include "INetwork.hpp"
#include <map>
#include <memory>

#define NB_CLIENTS_AUTHORIZED 4
#define MAXMESSAGE 255

namespace network {

    /**
     * @class Server
     * @brief Handles client connections and message exchanges for the R-Type game server.
     * The server listens for incoming client connections, manages multiple clients,
     * and handles message sending/receiving operations using the asio library.
     */
    class Server : virtual public network::INetwork {
      public:
        Server(const std::string &host, const std::size_t &port, asio::thread_pool &pool);
        ~Server();

        sig_atomic_t handleNetwork(const sig_atomic_t &new_status) override;
        void sendMessage(const std::string &message, uint8_t messageType) override;
        void sendUpdatedData(ecs::Registry &reg, std::list<DataToSend> &lastDataReceivedList) override;
        void clearData() override;
        std::list<DataToSend> getData() override;
        bool isConnectedInNetwork() override;
        network::Type getNetworkType() override;

        std::size_t getPlayerId() override {return 0;}
        std::size_t checkConnectedHeartbeat() override;

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
        bool _isRunning;                          ///< Indicates whether the server is still running.

        std::size_t _idClient;          ///< The ID of the last connected client.
        asio::thread_pool &_threadPool; ///< Thread pool for handling tasks.

        std::map<std::size_t, asio::ip::udp::endpoint> _connectedClients; ///< Map of connected clients and their endpoints.
        std::unordered_map<std::size_t, bool> _areClientsWorldSet;        ///< Map indicating whether each client's world is initialized.

        std::unordered_map<std::size_t, std::chrono::steady_clock::time_point> _lastAliveClient; ///< Heartbeat timestamps for clients.
        std::unordered_map<std::size_t, bool>
            _isClientStillHere; ///<  Map that set to false the a bool if the client with the std::size_t id was disconnected
        std::unordered_map<std::size_t, std::size_t> _corresOwnPlayerId; ///< Create a coresspondence between the _OwnID and the _playerID;

        std::list<DataToSend> _data;  ///< A list of Data received from clients, such as players updates.

        const uint16_t _magicNumber; ///< Magic number used for message validation.
        uint16_t _receivedMagicNbr;  ///< Received magic number for message validation.
        uint8_t _messageType;        ///< Type of the received message.
        uint8_t _messageId;          ///< ID of the received message.
        uint16_t _messageLen;        ///< Length of the received message.
    };
};

extern "C"
{
    network::Server* entryPoint(const std::string &host, const std::size_t &port, asio::thread_pool &pool) {
        return new network::Server(host, port, pool);
    }
}

#endif
