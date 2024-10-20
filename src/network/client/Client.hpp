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

#include "INetwork.hpp"
#include <string>
#include <thread>

#define MAXMESSAGE 255 ///< Maximum number of messages that can be sent before resetting the message ID.

namespace network {

    /**
     * @class Client
     * @brief Manages the client's network communication with the server.
     * The `Client` class handles communication with the server through a UDP socket. It sends and receives
     * messages, maintains the connection using heartbeat messages, and stores the player's game data received from the server.
     */
    class Client : virtual public network::INetwork {
      public:
        Client(const std::string &host, const std::string &port);
        ~Client();

        sig_atomic_t handleNetwork(const sig_atomic_t &new_status) override;
        void sendMessage(const std::string &message, uint8_t messageType) override;
        void sendUpdatedData(ecs::Registry &reg, std::list<DataToSend> &lastDataReceivedList) override;
        void clearData() override;
        std::list<DataToSend> getData() override;
        bool isConnectedInNetwork() override;
        network::Type getNetworkType() override;

        std::size_t getPlayerId() override;
        std::size_t checkConnectedHeartbeat() override {return 0;}

      private:

        void msgFromServer();
        void heartbeatToServer();
        void handleCtrlC(int signal);

        asio::io_context _ioContext;
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

extern "C"
{
    network::Client* entryPoint(const std::string &host, const std::string &port) {
        return new network::Client(host, port);
    }
}

#endif /* !CLIENT_HPP_ */
