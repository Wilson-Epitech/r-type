/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** main
*/

#include "RtypeGameServer.hpp"
#include "RunGame.hpp"
#include <iostream>

/*
--difficulty easy / medium / hard
passer en paran av[3] si ac == 4 à rtype
*/

int main(int ac, char **av) {
    if (ac < 2 || ac == 3 || ac > 4 || av[1] == NULL) {
        std::cerr << "Usage: ./server [port]" << std::endl; // à modif
        return -1;
    }
    if (ac == 4) {
        std::string flag = av[2];
        if (flag.compare("--difficulty") != 0) {
            std::cerr << "Usage: ./server [port]" << std::endl; // à modif
            return -1;
        }
    }

    try {
        asio::io_context io_context;
        asio::thread_pool pool(NB_CLIENTS_AUTHORIZED);
        std::unique_ptr<network::Server> server = std::make_unique<network::Server>(io_context, std::stoi(av[1]), pool);
        game::RtypeGameServer rtype;
        if (ac == 4)
            rtype.setGameDifficulty(av[3]);
        RunGame runGame(server, rtype.getRegistry(), rtype);
        runGame.run();
        pool.join();
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return -1;
    }
}
