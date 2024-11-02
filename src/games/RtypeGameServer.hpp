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
#include "BossFiringSystem.hpp"
#include "BossSpawnSystem.hpp"
#include "CollisionSystem.hpp"
#include "ControlSystem.hpp"
#include "EnemyFiringSystem.hpp"
#include "EnemySpawnSystem.hpp"
#include "FiringSystem.hpp"
#include "KillOffscreenEntitiesSystem.hpp"
#include "LifeSystem.hpp"
#include "PlayerSpawnSystem.hpp"
#include "PositionSystem.hpp"
#include "PowerupSystem.hpp"
#include "ResetForNextLevel.hpp"
#include "ScoreSystem.hpp"
#include "SpriteAnimationSystem.hpp"
#include "WeaponSystem.hpp"

#define MAX_SPAWN_HEIGHT 1040 ///< Maximum spawn height for enemies.
#define MIN_SPAWN_HEIGHT 30   ///< Minimum spawn height for enemies.
#define MAX_SPAWN_WIDTH 2420  ///< Maximum spawn width for enemies.
#define MIN_SPAWN_WIDTH 1920  ///< Minimum spawn width for enemies.

#define PLAYER_TEMPLATE_NAME "player_template"   ///< Defines the name of the player template entity.
#define PLAYER_TEMPLATE_POSSIBLE_VELOCITY 8      ///< Possible velocity for the player template entity.
#define PLAYER_TEMPLATE_SHOOTING_RATE 0.3        ///< The shooting rate for the player template entity.
#define PLAYER_TEMPLATE_NB_LIVES 5               ///< The number of lives for the player template entity.
#define PLAYER_TEMPLATE_DISPLAY_LIFE true        /// Indicates if the life of the player template entity has to be displayed.
#define PLAYER_TEMPLATE_SCORE_INCREMENTATION 100 ///< The score incrementation for the player template entity.
#define PLAYER_TEMPLATE_TEXT_CONTENT "Score:"    ///< The text content for the player template entity.
#define PLAYER_TEMPLATE_TEXT_X 50                ///< The text x position for the player template entity.
#define PLAYER_TEMPLATE_TEXT_Y 50                ///< The text y position for the player template entity.
#define PLAYER_TEMPLATE_TEXT_SIZE 20             ///< The text size for the player template entity.

#define PLAYER_SHOT_TEMPLATE_NAME "player_shot_template" ///< Defines the name of the player shot template entity.
#define PLAYER_SHOT_TEMPLATE_VX 10                       ///< The horizontal velocity for the player shot template entity.
#define PLAYER_SHOT_TEMPLATE_VY 0.0                      ///< The vertical velocity for the player shot template entity.

#define SPAWN_ENEMY_TEMPLATE_NAME "enemy_template" ///< Defines the name of the enemies spawn template entity.
#define ENEMY_VX -1                                ///< The horizontal velocity of enemies.
#define ENEMY_VY 1.5                               ///< The vertical velocity of enemies.
#define ENEMY_NB_LIVES 1                           ///<  The number of lives for enemies.
#define ENEMY_DISPLAY_LIFE false                   ///< Indicates if the life of enemies has to be displayed.
#define ENEMY_HAS_TO_SUMMON_POWERUP true           ///< Indicates if a powerup has a chance to spawn after death of enemies.
#define ENEMY_PATTERN_AMPLITUDE 60                 ///< Time amplitude of enemies pattern.
#define ENEMY_IS_REVERSIBLE true                   ///<
#define ENEMY_WAVE_SIZE 5                          ///< The size of each wave of enemies.
#define ENEMY_WAVE_SIZE_DIFFICULTY_FACTOR                                                                                                  \
    2                              ///< The factor that will be multiplied by the difficulty mode of the game for ENEMY_WAVE_SIZE define.
#define ENEMY_SHOOTING_RATE 3      ///< The shooting rate for enemies.
#define TIME_BEFORE_SPAWN_ENEMY 15 ///< Time before enemies start spawning.

#define SPAWN_STRONGER_ENEMY_TEMPLATE_NAME "stronger_enemy_template" ///< Defines the name of the stronger enemies spawn template entity.
#define STRONGER_ENEMY_VX -2                                         ///< The horizontal velocity of stronger enemies.
#define STRONGER_ENEMY_VY 0.0                                        ///< The vertical velocity of stronger enemies.
#define STRONGER_ENEMY_WAVE_SIZE 1                                   ///< The number of stronger enemies spawned in each wave.
#define STRONGER_ENEMY_SHOOTING_RATE 6                               ///< The shooting rate for stronger enemies.
#define STRONGER_ENEMY_NB_LIVES 10                                   ///< The number of lives for stronger enemies.
#define TIME_BEFORE_SPAWN_STRONGER_ENEMY 10                          ///< Time before stronger enemies start spawning.

#define ENEMY_SHOT_TEMPLATE_NAME "enemy_shot_template" ///< Defines the name of the enemies shot template entity.
#define ENEMY_SHOT_TEMPLATE_VX -10                     ///< The horizontal velocity for the enemy shot template entity.
#define ENEMY_SHOT_TEMPLATE_VY 0.0                     ///< The vertical velocity for the enemy shot template entity.

#define SPAWN_LEVEL1_BOSS_NAME "level1_boss" ///< Defines the name of the boss of level 1 spawn entity.
#define LEVEL1_BOSS_NB_LIVES 100             ///< The number of lives for the level 1 boss.
#define LEVEL1_BOSS_SHOOTING_RATE 0.5        ///< The shooting rate for the level 1 boss.

#define LEVEL1_BOSS_SHOT_NAME "level1_boss_shot" ///< Defines the name of the level 1 boss shot entity.
#define LEVEL1_BOSS_SHOT_VX -10                  ///< The horizontal velocity for the boss shot.
#define LEVEL1_BOSS_SHOT_VY 0.0                  ///< The vertical velocity for the boss shot.

#define POWER_UP_TEMPLATE_NAME "powerup_template" ///< Defines the name of the powerup template entity.
#define POWERUP_TEMPLATE_TEXT_CONTENT "null"      ///< The text content for the powerup template entity.
#define POWERUP_TEMPLATE_TEXT_SIZE 20             ///< The text size for the powerup template entity.

#define BACKGROUND_NAME "background" ///< Defines the name of the background entity.
#define BACKGROUND_VX -0.5           ///< The horizontal velocity for the background entity.
#define BACKGROUND_VY 0.0            ///< The vertical velocity for the background entity.

#define MUSIC_NAME "music" ///< Defines the name of the background music entity.
#define MUSIC_VOLUME 0.5   ///< The volume of the background music.

#define TEAM_SCORE_NAME "team_score"          ///< Defines the name of the team score entity.
#define TEAM_SCORE_TEXT_CONTENT "Team_score:" ///< The text content for the team score entity.
#define TEAM_SCORE_TEXT_X 50                  ///< The text x position for the team score entity.
#define TEAM_SCORE_TEXT_Y 100                 ///< The text y position for the team score entity.
#define TEAM_SCORE_TEXT_SIZE 20               ///< The text size for the team score entity.

#define TITLE_NAME "title"            ///< Defines the name of the title entity.
#define TITLE_TEXT_CONTENT "RType:"   ///< The text content for the title entity.
#define TITLE_TEXT_X SCREEN_WIDTH / 2 ///< The text x position for the title entity.
#define TITLE_TEXT_Y 50               ///< The text y position for the title entity.
#define TITLE_TEXT_SIZE 20            ///< The text size for the title entity.

#define WEAPON_NAME "weapon_template"
#define FRONT_WEAPON_NAME "front_weapon_sprite"
#define BACK_WEAPON_NAME "back_weapon_sprite"

namespace game {

    /**
     * @enum DifficultyMode
     * @brief Enum representing the differents difficulty modes of the game.
     */
    enum DifficultyMode {
        NO_MODE, ///< No mode
        EASY,    ///< Easy mode
        MEDIUM,  ///< Medium mode
        HARD     ///< Hard mode
    };

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
         * @param difficultyMode The mode of difficulty chosen.
         * @param gameName The name of the game, used for server-side management.
         *
         */
        RtypeGameServer(const std::string difficultyMode = "medium", const std::string gameName = "R-Type");
        ~RtypeGameServer() = default;

        /** @brief Updates the difficulty mode of the game.
         *  @param difficultyMode  The mode of difficulty chosen.
         */
        void setGameDifficulty(const std::string &difficultyMode) {
            auto it = _stringToDifficultyMode.find(difficultyMode);
            if (it == _stringToDifficultyMode.end()) {
                throw Error("Invalid difficulty mode");
            }
            _mode = it->second;
        }

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

        /**
         * @brief Initialize the string to difficulty mode map.
         * It links the easy, medium, and hard difficulty mode enum values to correspondent strings.
         */
        void initStringToDifficultyMode() {
            _stringToDifficultyMode.insert({"easy", game::DifficultyMode::EASY});
            _stringToDifficultyMode.insert({"medium", game::DifficultyMode::MEDIUM});
            _stringToDifficultyMode.insert({"hard", game::DifficultyMode::HARD});
        }

        DifficultyMode _mode; ///< The mode of difficulty of the current game instance.
        std::unordered_map<std::string, DifficultyMode>
            _stringToDifficultyMode; ///< An unordered map linking a string to a difficulty mode.
    };
}

#endif /* !RTYPEGAMESERVER_HPP_ */
