/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** RTypeGame
*/

#ifndef RTYPEGAME_HPP_
#define RTYPEGAME_HPP_

#include "AGame.hpp"
#include "CollisionSystem.hpp"
#include "ControlSystem.hpp"
#include "DrawSystem.hpp"
#include "EnemyFiringSystem.hpp"
#include "EnemySpawnSystem.hpp"
#include "FiringSystem.hpp"
#include "LifeSystem.hpp"
#include "PositionSystem.hpp"
#include "SoundSystem.hpp"
#include "SpriteAnimationSystem.hpp"

#define PLAYER_NAME "player"
#define BACKGROUND_NAME "background"
#define SPAWN_ENEMY_NAME "enemy"
#define SPAWN_BOSS_NAME "boss"
#define PLAYER_SHOOT "player_shoot"
#define ENEMY_SHOOT "enemy_shoot"

#define PLAYER_SPRITE_NB_PATTERN 16
#define FIRST_ENEMY_SPRITE_NB_PATTERN 8
#define SECOND_ENEMY_SPRITE_NB_PATTERN 12

#define PLAYER_SPRITE_FIRST_PATTERN 8
#define SECOND_ENEMY_SPRITE_FIRST_PATTERN 0

#define PLAYER_SPRITE_SCALE_FACTOR 2.3
#define SECOND_ENEMY_SPRITE_SCALE_FACTOR 2.0
#define ENEMY_SCALE_FACTOR 1
#define BOSS_SCALE_FACTOR 1.5

#define ENEMY_SHOT_SACLE_FACTOR 2

#define MAX_SPAWN_HEIGHT 1040
#define MIN_SPAWN_HEIGHT 30
#define MAX_SPAWN_WIDTH 2420
#define MIN_SPAWN_WIDTH 1920
#define TIME_BEFORE_SPAWN_ENEMY 10
#define ENEMY_WAVE_SIZE 8
#define ENEMY_VX -1
#define ENEMY_VY 0.0

#define BOSS_WAVE_SIZE 1
#define TIME_BEFORE_SPAWN_BOSS 15

namespace game {
    class RTypeGame : virtual public game::AGame {
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
