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
#define ERROR_VALUE 84 ///< Return value for errors
#define INVALID_ARGUMENTS_MESSAGE  "Usage: ./server [port] --difficulty [difficultyMode]\n\tdifficultyMode can be 'easy', 'medium', or 'hard' (it is set to medium by default)" ///< The message to display when arguments passed in parameter are invalid

int main(int ac, char **av) {
    if (ac < 2 || ac == 3 || ac > 4 || av[1] == NULL) {
        std::cerr << INVALID_ARGUMENTS_MESSAGE << std::endl;
        return ERROR_VALUE;
    }
    if (ac == 4) {
        std::string flag = av[2];
        if (flag.compare("--difficulty") != 0) {
            std::cerr << INVALID_ARGUMENTS_MESSAGE << std::endl;
            return ERROR_VALUE;
        }
    }

    try {
        game::RtypeGameServer rtype;
        if (ac == 4)
            rtype = game::RtypeGameServer(av[3]);
        asio::thread_pool pool(NB_CLIENTS_AUTHORIZED);
        RunGame runGame(rtype.getRegistry(), rtype, std::stoi(av[1]), pool);
        runGame.run();
        pool.join();
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return ERROR_VALUE;
    }
}
