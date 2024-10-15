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

#include "HandleData.hpp"
#include <array>
#include <asio.hpp>
#include <map>
#include <memory>

#define UPDATE_SERVER 0.02
#define NB_CLIENTS_AUTHORIZED 4
#define MAXMESSAGE 255

namespace network {

    /**
     * @class Server
     * @brief Handles client connections and message exchanges for the R-Type game server.
     * The server listens for incoming client connections, manages multiple clients,
     * and handles message sending/receiving operations using the asio library.
     */
    class Server {
      public:
        /**
         * @brief Constructs a Server object, binds the socket, and begins receiving messages.
         * @param ioContext The asio context to manage asynchronous operations.
         * @param host The IP address of the server.
         * @param port The port on which the server listens.
         * @param pool A thread pool to manage concurrent tasks.
         */
        Server(asio::io_context &ioContext, std::string host, std::size_t port, asio::thread_pool &pool);

        /**
         * @brief Destructor that stops the server and joins the IO thread.
         */
        ~Server();

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
         * @brief Retrieves the currently connected clients.
         * @return A map containing client IDs and their corresponding UDP endpoints.
         */
        std::map<std::size_t, asio::ip::udp::endpoint> getConnectedClients();

        /**
         * @brief Retrieves the number of currently connected clients.
         * @return The number of connected clients.
         */
        std::size_t getNbClients();

        /**
         * @brief Retrieves the ID of the last client that connected.
         * @return The ID of the most recently connected client.
         */
        std::size_t getLastConnectedClientId();

        /**
         * @brief Retrieves the map indicating whether each client's world has been set.
         * @return A map of client IDs and their world initialization status.
         */
        std::unordered_map<std::size_t, bool> getAreClientsWorldSet();

        /**
         * @brief Retrieves the entity data sent from clients.
         * @return A DataToSend structure containing entity data.
         */
        DataToSend getEntityData();

        /**
         * @brief Checks whether the received data has been processed.
         * @return A boolean indicating whether the data has been used.
         */
        bool getDataHasBeenUsed();

        /**
         * @brief Sets the flag indicating whether the received data has been processed.
         * @param value A boolean to set the data usage status.
         */
        void setDataHasBeenUsed(bool value);

        /**
         * @brief Retrieves the heartbeat timestamps for each connected client.
         * @return A map of client IDs and their last heartbeat timestamps.
         */
        std::unordered_map<std::size_t, std::chrono::steady_clock::time_point> getHeartbeatClient();

        /**
         * @brief Updates the heartbeat timestamps for clients.
         * @param map A map containing the updated heartbeat timestamps.
         */
        void setHeartBeatMap(std::unordered_map<std::size_t, std::chrono::steady_clock::time_point>);

        std::unordered_map<std::size_t, bool> getClientStillHere();
        void setClientStillHere(std::unordered_map<std::size_t, bool>);
        std::unordered_map<std::size_t, std::size_t> getCorrespondence();
        std::size_t getStillAliveClientNumber();

      private:
        /**
         * @brief Begins receiving data asynchronously from clients.
         */
        void startReceiving();

        /**
         * @brief Deserializes an incoming message received from a client.
         * @param message The message string to deserialize.
         */
        void deserializeMessage(const std::string message);

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

        std::thread _ioThread;                   ///< The thread running the asio context.
        asio::ip::udp::socket _socket;           ///< The UDP socket used for communication.
        asio::ip::udp::endpoint _remoteEndpoint; ///< Endpoint of the client sending data.
        std::array<char, 50000> _receiveBuffer;  ///< Buffer for storing received data.

        std::size_t _idClient;          ///< The ID of the last connected client.
        asio::thread_pool &_threadPool; ///< Thread pool for handling tasks.

        std::map<std::size_t, asio::ip::udp::endpoint> _connectedClients; ///< Map of connected clients and their endpoints.
        std::unordered_map<std::size_t, bool> _areClientsWorldSet;        ///< Map indicating whether each client's world is initialized.

        std::unordered_map<std::size_t, std::chrono::steady_clock::time_point> _lastAliveClient; ///< Heartbeat timestamps for clients.
        std::unordered_map<std::size_t, bool> _isClientStillHere;
        std::unordered_map<std::size_t, std::size_t> _corresOwnPlayerId;

        DataToSend _data;      ///< Data structure containing entity updates from clients.
        bool _dataHasBeenUsed; ///< Flag indicating whether the received data has been processed.

        const uint16_t _magicNumber; ///< Magic number used for message validation.
        uint16_t _receivedMagicNbr;  ///< Received magic number for message validation.
        uint8_t _messageType;        ///< Type of the received message.
        uint8_t _messageId;          ///< ID of the received message.
        uint16_t _messageLen;        ///< Length of the received message.
    };
};

#endif
