/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** main
*/

#include "RtypeGameServer.hpp"
#include "RunGame.hpp"
#include <iostream>

#define NB_CLIENTS_AUTHORIZED 4 ///< Number of maximum clients authorized to connect to the server
#define ERROR_VALUE 84          ///< Return value for errors
#define GAME_OPTION_START 2     ///< The game option starting position

int main(int ac, char **av) {
    std::vector<std::string> arguments;

    if (ac % 2 != 0 || ac > 6) {
        std::cerr << INVALID_ARGUMENTS_MESSAGE << std::endl;
        return ERROR_VALUE;
    }
    for (std::size_t i = GAME_OPTION_START; i < static_cast<std::size_t>(ac); i++)
        arguments.push_back(av[i]);
    try {
        game::RtypeGameServer rtype = game::RtypeGameServer(arguments);
        asio::thread_pool pool(NB_CLIENTS_AUTHORIZED);
        RunGame runGame(rtype.getRegistry(), rtype, std::stoi(av[1]), pool);
        runGame.run();
        pool.join();
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return ERROR_VALUE;
    }
}
