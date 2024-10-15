/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** main
*/

#include "RtypeGameServer.hpp"
#include "RunGame.hpp"
#include "raylib.h"
#include <iostream>

int main(int ac, char **av) {
    if (ac != 3 || av[1] == NULL || av[2] == NULL) {
        std::cerr << "Usage: ./server [ip] [port]" << std::endl;
        return -1;
    }

    try {
        asio::io_context io_context;
        asio::thread_pool pool(NB_CLIENTS_AUTHORIZED);
        std::unique_ptr<network::Server> server = std::make_unique<network::Server>(io_context, av[1], std::stoi(av[2]), pool);
        game::RtypeGameServer rtype;
        RunGame runGame(server, rtype.getRegistry(), rtype);
        runGame.run();
        pool.join();
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return -1;
    }
}
