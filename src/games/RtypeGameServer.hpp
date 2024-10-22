/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** RTypeGameServer
*/

/**
 * @file RtypeGameServer.hpp
 * @brief Declares the `RtypeGameServer` class for managing the server-side logic of the R-Type game.
 * This file defines the `RtypeGameServer` class, which is responsible for handling all the server-side
 * operations, including entity spawning, component registration, system initialization, and maintaining
 * the game state.
 */

#ifndef RTYPEGAMESERVER_HPP_
#define RTYPEGAMESERVER_HPP_

#include "AGame.hpp"
#include "CollisionSystem.hpp"
#include "ControlSystem.hpp"
#include "EnemyFiringSystem.hpp"
#include "EnemySpawnSystem.hpp"
#include "FiringSystem.hpp"
#include "KillOffscreenEntitiesSystem.hpp"
#include "LifeSystem.hpp"
#include "PlayerSpawnSystem.hpp"
#include "PositionSystem.hpp"
#include "SpriteAnimationSystem.hpp"
#include "ScoreSystem.hpp"
#include "PowerupSystem.hpp"

/** @brief Defines the name of the player entity */
#define PLAYER_NAME "player"
/** @brief Defines the name of the player shot entity */
#define PLAYER_SHOT "player_shot"
/** @brief Defines the name of the background entity */
#define BACKGROUND_NAME "background"
/** @brief Defines the name of the enemy spawn entity */
#define SPAWN_ENEMY_NAME "enemy"
/** @brief Defines the name of the enemy shot entity */
#define ENEMY_SHOT "enemy_shot"
/** @brief Defines the name of the boss spawn entity */
#define SPAWN_BOSS_NAME "boss"
/** @brief Defines the name of the background music entity */
#define MUSIC_NAME "music"
/** @brief Defines the name of the team score entity */
#define TEAM_SCORE_NAME "team_score"
/** @brief Defines the name of the title entity */
#define TITLE_NAME "title"
/** @brief Defines the name of the powerup entity */
#define POWERUP_NAME "powerup"

/** @brief Maximum spawn height for enemies */
#define MAX_SPAWN_HEIGHT 1040
/** @brief Minimum spawn height for enemies */
#define MIN_SPAWN_HEIGHT 30
/** @brief Maximum spawn width for enemies */
#define MAX_SPAWN_WIDTH 2420
/** @brief Minimum spawn width for enemies */
#define MIN_SPAWN_WIDTH 1920
/** @brief Time before enemies start spawning */
#define TIME_BEFORE_SPAWN_ENEMY 10
/** @brief The size of each wave of enemies */
#define ENEMY_WAVE_SIZE 8
/** @brief The horizontal velocity of enemies */
#define ENEMY_VX -1
/** @brief The vertical velocity of enemies */
#define ENEMY_VY 0.0

/** @brief The number of bosses spawned in each wave */
#define BOSS_WAVE_SIZE 1
/** @brief Time before bosses start spawning */
#define TIME_BEFORE_SPAWN_BOSS 15

namespace game {

    /**
     * @class RtypeGameServer
     * @brief Manages the server-side logic of the R-Type game.
     * The `RtypeGameServer` class is responsible for handling all server-side operations, including entity
     * spawning, component registration, system initialization, and maintaining the game state for players.
     */
    class RtypeGameServer : virtual public game::AGame {
      public:
        /**
         * @brief Constructs the RtypeGameServer and initializes the game.
         * @param gameName The name of the game, used for server-side management.
         */
        RtypeGameServer(const std::string gameName = "R-Type");
        ~RtypeGameServer() = default;

      private:
        /**
         * @brief Manages the game state and updates the registry accordingly.
         * This method is responsible for controlling the flow of the game and ensuring the
         * game state remains in sync with server-side operations.
         * @param reg The ECS registry that contains all entities and components.
         * @return The current game status.
         */
        GameStatus gameState(ecs::Registry &reg) override;

        /**
         * @brief Spawns the necessary entities for the R-Type game.
         * This method creates entities such as the player, player shots, enemies, bosses, background, and background music.
         */
        void spawnEntities() override;

        /**
         * @brief Registers components for the game entities.
         * This method registers all the required components like position, velocity, type, life, and others
         * in the ECS registry.
         */
        void registerComponents() override;

        /**
         * @brief Adds components to the created entities.
         * This method assigns the registered components (e.g., velocity, sprite info, shooting, sound effects)
         * to the corresponding game entities.
         */
        void addComponentsToEntities() override;

        /**
         * @brief Adds the required systems to the ECS registry.
         * This method registers the systems that control various aspects of the game such as movement, collision detection,
         * spawning enemies, shooting, and other game mechanics.
         */
        void addSystemsToRegistry() override;
    };
}

#endif /* !RTYPEGAMESERVER_HPP_ */
