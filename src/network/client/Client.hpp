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

#include "ANetwork.hpp"
#include <string>
#include <thread>

#define MAXMESSAGE 255   ///< Maximum number of messages that can be sent before resetting the message ID.
#define HEARTBEAT_TIME 1 ///< Time in second where the client has to send a heartbeat to the server.

namespace network {

    /**
     * @class Client
     * @brief Manages the client's network communication with the server.
     * The `Client` class handles communication with the server through a UDP socket. It sends and receives
     * messages, maintains the connection using heartbeat messages, and stores the player's game data received from the server.
     */
    class Client : virtual public network::ANetwork {
      public:
        /**
         * @brief Constructs a `Client` and establishes a connection to the server.
         * Initializes the client, opens a UDP socket, resolves the server address, and starts threads for
         * receiving messages and sending heartbeat messages.
         * @param host The server's hostname or IP address.
         * @param port The server's port number.
         */
        Client(const std::string &host, const std::string &port);
        /**
         * @brief Destructor for the `Client` class.
         * Stops the client by terminating threads and closing the socket.
         */
        ~Client();

        void sendMessage(const std::string &message, uint8_t messageType) override;
        void sendUpdatedData(ecs::Registry &reg, std::list<DataToSend> &lastDataReceivedList) override;
        std::size_t manageConnection() override;
        void updateData(ecs::Registry &reg) override;
        std::list<DataToSend> getData() override;
        network::Type getNetworkType() override;
        bool isConnectedInNetwork() override;

      private:
        /**
         * @brief Listens for and processes messages from the server.
         * This function runs in a separate thread and continuously listens for messages from the server. It processes
         * the messages and handles updates to the client's state.
         */
        void msgFromServer();
        /**
         * @brief Catch the ctrl C signal.
         * @param signal Retrieve the signal sent.
         */
        void handleCtrlC(int signal);

        asio::io_context _ioContext;
        asio::ip::udp::socket _socket;            ///< The UDP socket used for communication with the server.
        asio::ip::udp::endpoint _receiveEndpoint; ///< The server's endpoint (address and port).
        std::thread _receiverThread;              ///< Thread for receiving messages from the server.
        std::thread _heartbeatThread;             ///< Thread for sending heartbeat messages to the server.

        std::size_t _playerId; ///< The player's ID assigned by the server.
    };
};

/**
 * @brief Used to create a new client instance by loading a shared library from this class.
 * @param host The server's hostname or IP address.
 * @param port The server's port number.
 * @return network::Client* Pointer to the newly created client instance
 */
extern "C" {
network::Client *entryPoint(const std::string &host, const std::string &port) {
    return new network::Client(host, port);
}
}

#endif /* !CLIENT_HPP_ */
