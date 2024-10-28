/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** main
*/

#include "RtypeGameServer.hpp"
#include "RunGame.hpp"
#include <iostream>

#define NB_CLIENTS_AUTHORIZED 4

int main(int ac, char **av) {
    if (ac != 2 || av[1] == NULL) {
        std::cerr << "Usage: ./server [port]" << std::endl;
        return -1;
    }

    try {
        asio::thread_pool pool(NB_CLIENTS_AUTHORIZED);
        game::RtypeGameServer rtype;
        RunGame runGame(rtype.getRegistry(), rtype, std::stoi(av[1]), pool);
        runGame.run();
        pool.join();
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return -1;
    }
}
