/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** Client
*/

/**
 * @file Client.hpp
 * @brief Declares the `Client` class, responsible for managing the client-side network communication.
 * This file contains the declaration of the `Client` class, which manages sending and receiving messages
 * between the client and the server using the `asio` networking library. It also includes the setup for
 * maintaining the connection through heartbeat messages.
 */

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include "HandleData.hpp"
#include <array>
#include <asio.hpp>
#include <list>
#include <string>
#include <thread>

#define UPDATE_CLIENT 0.02 ///< Interval for updating the client in seconds.
#define MAXMESSAGE 255     ///< Maximum number of messages that can be sent before resetting the message ID.

namespace network {

    /**
     * @class Client
     * @brief Manages the client's network communication with the server.
     * The `Client` class handles communication with the server through a UDP socket. It sends and receives
     * messages, maintains the connection using heartbeat messages, and stores the player's game data received from the server.
     */
    class Client {
      public:
        /**
         * @brief Constructs a `Client` and establishes a connection to the server.
         * Initializes the client, opens a UDP socket, resolves the server address, and starts threads for
         * receiving messages and sending heartbeat messages.
         * @param ioContext The `asio::io_context` object used to manage asynchronous operations.
         * @param host The server's hostname or IP address.
         * @param port The server's port number.
         */
        Client(asio::io_context &ioContext, const std::string &host, const std::string &port);

        /**
         * @brief Destructor for the `Client` class.
         * Stops the client by terminating threads and closing the socket.
         */
        ~Client();

        /**
         * @brief Sends a message to the server.
         * Serializes the message and sends it over the UDP socket.
         * @param message The message to send.
         * @param messageType The type of the message, which is used for server-side processing.
         */
        void sendMessage(const std::string &message, uint8_t messageType);

        /**
         * @brief Catch the ctrl C signal.
         * @param signal Retrieve the signal sent.
         */
        static void handleCtrlC(int signal);
        sig_atomic_t handleClient(const sig_atomic_t new_status);

        /**
         * @brief Checks if the client is still running.
         * @return true If the client is active and running.
         * @return false If the client has stopped.
         */
        bool getIsRunning();

        /**
         * @brief Retrieves the data received from the server.
         * @return std::list<DataToSend> The data received from the server.
         */
        std::list<DataToSend> getData();

        /**
         * @brief Retrieves the player's ID.
         * @return std::size_t The player's ID.
         */
        std::size_t getPlayerId();
        /**
         * @brief Clear the list of Data received by the server
         */
        void clearData();
        void sendUpdatedData(ecs::Registry &reg, std::list<DataToSend> &lastDataReceivedList);

      private:
        /**
         * @brief Listens for and processes messages from the server.
         * This function runs in a separate thread and continuously listens for messages from the server. It processes
         * the messages and handles updates to the client's state.
         */
        void msgFromServer();

        /**
         * @brief Sends periodic heartbeat messages to the server to maintain the connection.
         * This function runs in a separate thread and sends heartbeat messages at regular intervals to notify the server
         * that the client is still connected and active.
         */
        void heartbeatToServer();

        asio::ip::udp::socket _socket;            ///< The UDP socket used for communication with the server.
        asio::ip::udp::endpoint _receiveEndpoint; ///< The server's endpoint (address and port).
        std::thread _receiverThread;              ///< Thread for receiving messages from the server.
        std::thread _heartbeatThread;             ///< Thread for sending heartbeat messages to the server.
        bool _isRunning;                          ///< Indicates whether the client is still running.

        const uint16_t _magicNumber; ///< Magic number used for message validation.
        uint8_t _messageId;          ///< ID of the current message being sent.
        uint16_t _len;               ///< Length of the current message.

        std::list<DataToSend> _data; ///< A list of Data received from the server, such as game state updates.
        std::size_t _playerId;       ///< The player's ID assigned by the server.
    };
};

#endif /* !CLIENT_HPP_ */
