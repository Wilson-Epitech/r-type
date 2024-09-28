/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** main
*/

/**
 * @brief this is the main function,
 * ...
 * @return int
 */

#include "GameEngine.hpp"
#include "RTypeGame.hpp"
#include <iostream>

int main() {
    try {
        game::RTypeGame rtype;
        GameEngine engine(rtype.getRegistry(), rtype.getGameName(), rtype);
        engine.run();
    } catch (const Error &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}
