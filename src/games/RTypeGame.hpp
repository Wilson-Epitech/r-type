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
    #include "SpriteAnimationSystem.hpp"
    #include "EnemySpawnSystem.hpp"

    #define PLAYER_NAME "player"
    #define BACKGROUND_NAME "background"
    #define SPAWN_ENEMY_NAME "enemy"
    #define FIRST_ENEMY_NAME "enemy1"
    #define SECOND_ENEMY_NAME "enemy2"
    #define THIRD_ENEMY_NAME "enemy3"
    #define PLAYER_SHOOT "player_shoot"

    #define PLAYER_SPRITE_NB_PATTERN 16
    #define FIRST_ENEMY_SPRITE_NB_PATTERN 8
    #define SECOND_ENEMY_SPRITE_NB_PATTERN 12

    #define PLAYER_SPRITE_FIRST_PATTERN 8
    #define SECOND_ENEMY_SPRITE_FIRST_PATTERN 0

    #define PLAYER_SPRITE_SCALE_FACTOR 2.3
    #define SECOND_ENEMY_SPRITE_SCALE_FACTOR 2.0
    #define ENEMY_SCALE_FACTOR 1

    #define MAX_SPAWN_HEIGHT 1070
    #define MIN_SPAWN_HEIGHT 10
    #define MAX_SPAWN_WIDTH 1930
    #define MIN_SPAWN_WIDTH 1920
    #define TIME_BEFORE_SPAWN_ENEMY 3
    #define ENEMY_VX -0.4
    #define ENEMY_VY 0.0

namespace game
{
    class RTypeGame : virtual public game::AGame
    {
        public:
            RTypeGame(const std::string gameName = "R-Type");
            ~RTypeGame() = default;

            GameStatus gameState(ecs::Registry &reg) override;
            void spawnEntities() override;
            void registerComponents() override;
            void addComponentsToEntities() override;
            void addSystemsToRegister() override;
            ecs::Registry &getRegister() override;
            std::string &getGameName() override;
    };
}

#endif /* !RTYPEGAME_HPP_ */
