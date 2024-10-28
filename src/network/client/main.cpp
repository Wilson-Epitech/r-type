/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** main
*/

#include "RtypeGameClient.hpp"
#include "RunGame.hpp"
#include <iostream>

int main(int ac, char **av) {
    if (ac != 3 || av[1] == NULL || av[2] == NULL) {
        std::cerr << "Usage: ./client [ip] [port]" << std::endl;
        return -1;
    }

    try {
        game::RtypeGameClient rtype;
        RunGame runGame(rtype.getRegistry(), rtype.getGameName(), rtype, av[1], av[2]);
        runGame.run();
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return -1;
    }
}
