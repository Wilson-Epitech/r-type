/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** server
*/

#include "Server.hpp"
#include <iostream>
#include <functional>

Server::Server(asio::io_context& io_context) : socket_(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), 4242)), idClient(1) {
    start_receive();
}

void Server::start_receive() {
    socket_.async_receive_from(
        asio::buffer(recv_buffer_), remote_endpoint_,
        std::bind(&Server::handle_receive, this,
        std::placeholders::_1,
        std::placeholders::_2));
}

void Server::handle_receive(const std::error_code& error, std::size_t)
{
    if (!error) {
        std::shared_ptr<std::string> message(new std::string("Server received\n"));

        socket_.async_send_to(asio::buffer(*message), remote_endpoint_,
            std::bind(&Server::handle_send, this, message,
                std::placeholders::_1,
                std::placeholders::_2));

        start_receive();
        std::cout << "Client: " << std::string(recv_buffer_.data()) << std::endl;
        std::fill(std::begin(recv_buffer_), std::end(recv_buffer_), 0);
    } else {
        std::cerr << "Receive error: " << error.message() << std::endl;
    }
}

void Server::handle_send(std::shared_ptr<std::string> message, const std::error_code& error, std::size_t)
{
    if (error)
        std::cerr << "Failed to send message. Error: " << error.message() << std::endl;
}

void Server::sendToAClient(const std::size_t idClient, const std::string message)
{

}

