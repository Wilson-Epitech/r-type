/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** Server
*/

#ifndef SERVER_HPP
#define SERVER_HPP

#include <array>
#include <memory>
#include <asio.hpp>

class Server
{
    public:
        Server(asio::io_context& ioContext, std::size_t port);
        void sendToAClient(const std::size_t idClient, const std::string message);
        void sendToAllClient(const std::string message);

    private:
        void startReceiving();
        void handleReceive(const std::error_code& error, std::size_t bytesTransferred);
        void handleSend(std::shared_ptr<std::string> message, const std::error_code& error, std::size_t bytesTransferred);
        std::size_t logNewConnection(asio::ip::udp::endpoint endpoint);

        asio::ip::udp::socket _socket;
        asio::ip::udp::endpoint _remoteEndpoint;
        std::array<char, 1024> _receiveBuffer;

        std::size_t _idClient;
        std::unordered_map<std::size_t, asio::ip::udp::endpoint> _connectedClients;
};

#endif

