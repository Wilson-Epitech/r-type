/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** Client
*/

#include <array>
#include <asio.hpp>
#include <string>
#include <thread>

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

class Client {
  public:
    Client(asio::io_context &ioContext, const std::string &host, const std::string &port);
    ~Client();
    void sendMessage(const std::string &message);
    bool getIsRunning();

  private:
    void msgFromServer();

    asio::ip::udp::socket _socket;
    asio::ip::udp::endpoint _receiveEndpoint;
    std::thread _receiverThread;
    bool _isRunning;
};

#endif /* !CLIENT_HPP_ */
