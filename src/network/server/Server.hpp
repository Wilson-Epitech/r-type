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

#define NB_CLIENTS_AUTHORIZED 4 ///< Number of maximum clients authorized
#define MAXMESSAGE 255 ///< Maximum number of messages that can be sent before resetting the message ID.
#define CHECK_HEARTBEAT_TIME                                                                                                               \
    5 ///< Time in second where the server has to disconnect a client if he hasn't received a heartbeat in this elapsed time.

namespace network {

    /**
     * @class Server
     * @brief Handles client connections and message exchanges for the R-Type game server.
     * The server listens for incoming client connections, manages multiple clients,
     * and handles message sending/receiving operations using the asio library.
     */
    class Server : virtual public network::ANetwork {
      public:
        /**
         * @brief Constructs a Server object, binds the socket, and begins receiving messages.
         * @param port The port on which the server listens.
         * @param pool A thread pool to manage concurrent tasks.
         */
        Server(const std::size_t &port, asio::thread_pool &pool);

        /**
         * @brief Destructor that stops the server and joins the IO thread.
         */
        ~Server();

        void sendMessage(const std::string &message, uint8_t messageType) override;
        void sendUpdatedData(ecs::Registry &reg, std::list<DataToSend> &lastDataReceivedList) override;
        std::size_t manageConnection() override;
        void updateData(ecs::Registry &reg) override;
        std::list<DataToSend> getData() override;
        network::Type getNetworkType() override;
        bool isConnectedInNetwork() override;

      private:
        /**
         * @brief Begins receiving data asynchronously from clients.
         */
        void startReceiving();

        /**
         * @brief Sends a message to a specific client.
         * @param idClient The ID of the client to send the message to.
         * @param message The message content to send.
         * @param messageType The type of message being sent.
         */
        void sendToAClient(const std::size_t idClient, const std::string message, uint8_t messageType);

        /**
         * @brief Sends a message to all connected clients.
         * @param message The message content to send.
         * @param messageType The type of message being sent.
         */
        void sendToAllClient(const std::string message, uint8_t messageType);

        /**
         * @brief Handles data reception from a client.
         * @param error The error code from the receive operation.
         * @param bytesTransferred The number of bytes transferred in the operation.
         */
        void handleReceive(const std::error_code &error, std::size_t bytesTransferred);

        /**
         * @brief Handles the sending of data to a client.
         * @param error The error code from the send operation.
         * @param bytesTransferred The number of bytes transferred in the operation.
         */
        void handleSend(const std::error_code &error, std::size_t bytesTransferred);

        /**
         * @brief Logs a new client connection and assigns a unique ID.
         * @param endpoint The UDP endpoint of the new client.
         * @return The ID assigned to the client.
         */
        std::size_t logNewConnection(asio::ip::udp::endpoint endpoint);

        /**
         * @brief Updates the heartbeat timestamps for clients.
         * @param map A map containing the updated heartbeat timestamps.
         */
        void setHeartBeatMap(std::unordered_map<std::size_t, std::chrono::steady_clock::time_point>);

        /**
         * @brief set the map _clientStillHere to new values.
         * @param unordered_map A map containing the list of all client once connected and if they are still connected.
         */
        void setClientStillHere(std::unordered_map<std::size_t, bool>);

        /**
         * @brief Retrieves the ID of the last client that connected.
         * @return The ID of the most recently connected client.
         */
        std::size_t getLastConnectedClientId();

        /**
         * @brief count the number of client that are still alive.
         * @return size_t a size_t containing the number of client stll alive.
         */
        std::size_t getStillAliveClientNumber();

        /**
         * @brief get the map _clientStillHere.
         * @return map A map containing the list of all client once connected and if they are still connected.
         */
        std::unordered_map<std::size_t, bool> getClientStillHere();

        /**
         * @brief get the map _corresOwnPlayerId.
         * @return unordered_map A map containing the list of all client once connected and their ID clients.
         */
        std::unordered_map<std::size_t, std::size_t> getCorrespondence();

        /**
         * @brief Retrieves the heartbeat timestamps for each connected client.
         * @return A map of client IDs and their last heartbeat timestamps.
         */
        std::unordered_map<std::size_t, std::chrono::steady_clock::time_point> getHeartbeatClient();

        asio::io_context _ioContext;
        std::thread _ioThread;                   ///< The thread running the asio context.
        asio::ip::udp::socket _socket;           ///< The UDP socket used for communication.
        asio::ip::udp::endpoint _remoteEndpoint; ///< Endpoint of the client sending data.
        std::array<char, 50000> _receiveBuffer;  ///< Buffer for storing received data.

        std::size_t _idClient;          ///< The ID of the last connected client.
        asio::thread_pool &_threadPool; ///< Thread pool for handling tasks.

        std::map<std::size_t, asio::ip::udp::endpoint> _connectedClients; ///< Map of connected clients and their endpoints.

        std::unordered_map<std::size_t, std::chrono::steady_clock::time_point> _lastAliveClient; ///< Heartbeat timestamps for clients.
        std::unordered_map<std::size_t, bool>
            _isClientStillHere; ///<  Map that set to false the bool if the client with the std::size_t id was disconnected
        std::unordered_map<std::size_t, std::size_t> _corresOwnPlayerId; ///< Create a coresspondence between the _OwnID and the _playerID;

        uint16_t _receivedMagicNbr; ///< Received magic number for message validation.
        uint8_t _messageType;       ///< Type of the received message.
    };
};

/**
 * @brief Used to create a new server instance by loading a shared library from this class.
 * @param port The port on which the server listens.
 * @param pool A thread pool to manage concurrent tasks.
 * @return network::Server* Pointer to the newly created server instance
 */
extern "C" {
network::Server *entryPoint(const std::size_t &port, asio::thread_pool &pool) {
    return new network::Server(port, pool);
}
}

#endif
