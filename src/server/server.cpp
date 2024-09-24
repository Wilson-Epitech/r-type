/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** server
*/

#include <iostream>
#include <asio.hpp>
#include <array>

#define PORT 4242

/**
 * @brief This function...
 *
 * @return int
 */
int main() {
    try {
        asio::io_context io_context;

        // create socket udp
        asio::ip::udp::socket socket(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), PORT));
        std::vector<char> buffer(1024);
        asio::ip::udp::endpoint client_endpoint;

        while (1) {
            std::size_t len = socket.receive_from(asio::buffer(buffer), client_endpoint);
            buffer.resize(len);
            std::cout << "Client: " << std::string(buffer.data(), len) << std::endl;
        }
        // std::string hello = "Hello from server";
        // socket.send_to(asio::buffer(hello), client_endpoint);
        // std::cout << "Hello message sent." << std::endl;
    } catch (std::exception & e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
    }
    return 0;
}

