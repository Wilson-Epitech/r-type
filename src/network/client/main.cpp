/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** main
*/

#include "Client.hpp"
#include <iostream>

int main(int ac, char** av)
{
    if (ac != 2 || av[1] == NULL) {
        std::cout << "Usage: ./client [port]" << std::endl;
        return -1;
    }

    try {
        asio::io_context ioContext;
        Client client(ioContext, "127.0.0.1", av[1]);
        std::string message;

        while (true) {
            std::getline(std::cin, message);

            if (message == "exit") {
                client.sendMessage("Client disconnected");
                break;
            }
            client.sendMessage(message);
        }

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
