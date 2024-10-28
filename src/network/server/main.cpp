/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** main
*/

#include "RtypeGameServer.hpp"
#include "RunGame.hpp"
#include <iostream>

#define ERROR_VALUE 84
#define INVALID_ARGUMENTS_MESSAGE  "Usage: ./server [port] | ./server [port] --difficulty difficultyMode\n\tdifficultyMode can be 'easy', 'medium', or 'hard' (set to medium by default)"

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
        asio::io_context io_context;
        asio::thread_pool pool(NB_CLIENTS_AUTHORIZED);
        std::unique_ptr<network::Server> server = std::make_unique<network::Server>(io_context, std::stoi(av[1]), pool);
        RunGame runGame(server, rtype.getRegistry(), rtype);
        runGame.run();
        pool.join();
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return ERROR_VALUE;
    }
}
