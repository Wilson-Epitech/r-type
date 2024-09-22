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

#include "RTypeGame.hpp"
#include "GameEngine.hpp"

int main()
{
    game::RTypeGame rtype;
    GameEngine engine(rtype.getRegister(), rtype.getGameName());

    engine.run();
    return 0;
}
