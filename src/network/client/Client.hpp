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

#define UPDATE_CLIENT 0.02

class Client {
  public:
    Client(asio::io_context &ioContext, const std::string &host, const std::string &port);
    ~Client();
    void sendMessage(const std::string &message);
    bool getIsRunning();
    DataToSend getData();

  private:
    void msgFromServer();

    asio::ip::udp::socket _socket;
    asio::ip::udp::endpoint _receiveEndpoint;
    std::thread _receiverThread;
    bool _isRunning;

    DataToSend _data;
};

#endif /* !CLIENT_HPP_ */
