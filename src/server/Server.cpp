/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** server
*/

#include "Server.hpp"
#include <iostream>
#include <functional>

Server::Server(asio::io_context& io_context) : socket_(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), 4242)), _idClient(0) {
    start_receive();
}

void Server::start_receive() {
    socket_.async_receive_from(
        asio::buffer(recv_buffer_), remote_endpoint_,
        std::bind(&Server::handle_receive, this,
        std::placeholders::_1,
        std::placeholders::_2));
}

void Server::handle_receive(const std::error_code& error, std::size_t byteTransfered)
{
    if (!error) {
        std::size_t talkingClient = logNewConnection(remote_endpoint_);
        std::cout << "Server received msg from client " << talkingClient << ": " << std::string(recv_buffer_.data(), byteTransfered) << std::endl;

        std::shared_ptr<std::string> message(new std::string("Server received\n"));

        socket_.async_send_to(asio::buffer(*message), remote_endpoint_,
            std::bind(&Server::handle_send, this, message,
                std::placeholders::_1,
                std::placeholders::_2));

        start_receive();
        recv_buffer_.fill(0);
    } else {
        std::cerr << "Receive error: " << error.message() << std::endl;
    }
}

void Server::handle_send(std::shared_ptr<std::string> message, const std::error_code& error, std::size_t)
{
    if (error)
        std::cerr << "Failed to send message. Error: " << error.message() << std::endl;
}

void Server::sendToAClient(const std::size_t idClient, const std::string messageToSend)
{
    auto it = _connectedClients.find(idClient);

    if (it != _connectedClients.end()) {
        std::shared_ptr<std::string> message(new std::string (messageToSend));

        socket_.async_send_to(asio::buffer(*message), remote_endpoint_,
            std::bind(&Server::handle_send, this, message,
                std::placeholders::_1,
                std::placeholders::_2));
    } else {
        std::cerr << "Client " << idClient << "not found" << std::endl;
    }
}

std::size_t Server::logNewConnection(asio::ip::udp::endpoint endpoint)
{
    for (const auto& it: _connectedClients) {
        if (it.second == endpoint)
            return it.first;
    }

    _idClient++;
    _connectedClients[_idClient] = endpoint;
    std::cout << "New client connected: " << _idClient << std::endl;
    return _idClient;

}
