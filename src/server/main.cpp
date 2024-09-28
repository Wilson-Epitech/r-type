/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** main
*/

#include <iostream>
#include "Server.hpp"

int main(int ac, char **av)
{
    if (ac != 2 || av[1] == NULL) {
        std::cout << "Usage: ./client [port]" << std::endl;
        return -1;
    }
    try {
        asio::io_context io_context;
        Server server(io_context, std::stoi(av[1]));
        io_context.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
