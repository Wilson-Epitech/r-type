/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** main
*/

#include "RtypeGameClient.hpp"
#include "RunGame.hpp"
#include <iostream>

#define ERROR_VALUE 84

int main(int ac, char **av) {
    if (ac != 3 || av[1] == NULL || av[2] == NULL) {
        std::cerr << "Usage: ./client [ip] [port]" << std::endl;
        return ERROR_VALUE;
    }

    try {
        asio::io_context ioContext;
        std::unique_ptr<network::Client> client = std::make_unique<network::Client>(ioContext, av[1], av[2]);
        game::RtypeGameClient rtype;
        RunGame runGame(client, rtype.getRegistry(), rtype.getGameName(), rtype);
        runGame.run();
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return ERROR_VALUE;
    }
}
