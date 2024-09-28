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
        Server(asio::io_context& io_context);
        void sendToAClient(const std::size_t idClient, const std::string message);

    private:
        void start_receive();
        void handle_receive(const std::error_code& error, std::size_t bytes_transferred);
        void handle_send(std::shared_ptr<std::string> message, const std::error_code& error, std::size_t bytes_transferred);

        asio::ip::udp::socket socket_;
        asio::ip::udp::endpoint remote_endpoint_;
        std::array<char, 1024> recv_buffer_;

        std::size_t idClient;
        std::unordered_map<std::size_t, asio::ip::udp::endpoint> connectedClients;
};

#endif

