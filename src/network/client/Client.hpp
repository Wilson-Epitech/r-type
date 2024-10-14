/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** Client
*/

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include "Lib.hpp"
#include <array>
#include <asio.hpp>
#include <string>
#include <thread>
#include <list>

#define MAXMESSAGE 255

namespace network {
    class Client {
      public:
        Client(asio::io_context &ioContext, const std::string &host, const std::string &port);
        ~Client();

        static void handleCtrlC(int signal);
        sig_atomic_t handleClient(const sig_atomic_t new_status);
        void sendMessage(const std::string &message, uint8_t messageType);
        bool getIsRunning();
        std::list<DataToSend> getData();
        std::size_t getPlayerId();
        void clearData();
        void sendUpdatedData(ecs::Registry &reg, std::list<DataToSend> &lastDataReceivedList);

      private:
        void msgFromServer();
        void heartbeatToServer();

        asio::ip::udp::socket _socket;
        asio::ip::udp::endpoint _receiveEndpoint;
        std::thread _receiverThread;
        std::thread _heartbeatThread;
        bool _isRunning;

        const uint16_t _magicNumber;
        uint8_t _messageId;
        uint16_t _len;

      std::list<DataToSend> _data;
      std::size_t _playerId;
  };
};

#endif /* !CLIENT_HPP_ */
