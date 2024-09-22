/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** RTypeGame
*/

#ifndef RTYPEGAME_HPP_
    #define RTYPEGAME_HPP_

    #include "AGame.hpp"
    #include "ControlSystem.hpp"
    #include "PositionSystem.hpp"
    #include "CollisionSystem.hpp"
    #include "LifeSystem.hpp"
    #include "DrawSystem.hpp"
    #include "FiringSystem.hpp"

    #define PLAYER_NAME "player"
    #define FIRST_ENEMY_NAME "enemy1"
    #define SECOND_ENEMY_NAME "enemy2"
    #define THIRD_ENEMY_NAME "enemy3"

namespace game
{
    class RTypeGame : virtual public game::AGame
    {
        public:
            RTypeGame(const std::string gameName = "R-Type");
            ~RTypeGame() = default;

            void spawnEntities() override;
            void registerComponents() override;
            void addComponentsToEntities() override;
            void addSystemsToRegister() override;
            ecs::Registry &getRegister() override;
            std::string &getGameName() override;
    };
}

#endif /* !RTYPEGAME_HPP_ */
