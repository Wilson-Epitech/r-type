/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** BossSpawnSystem
*/

/**
 * @file BossSpawnSystem.hpp
 * @brief Declares the system responsible for spawning boss entities in the game engine ECS.
 * This file contains the `BossSpawnSystem` class, which handles the logic of spawning the
 * boss at the end of the level.
 */

#ifndef BOSSSPAWNSYSTEM_HPP_
#define BOSSSPAWNSYSTEM_HPP_

#include "Assets.hpp"
#include "Components.hpp"
#include "ISystem.hpp"
#include "Assets.hpp"

#define LEVEL1_BOSS_STOP 1200    ///< Set the stop position for the boss' entering.
#define LEVEL1_BOSS_SPAWN_X 1940 ///< Sets the position in width for the appearance of the boss.
#define LEVEL1_BOSS_SPAWN_Y 150  ///< Sets the position in height for the appearance of the boss.
#define LEVEL1_BOSS_VELOCITY -1  ///< Sets the velocity for the appearance of the boss.
#define TIME_WAIT_LEVEL1_BOSS 10 ///< Sets the time to wait before making the level1 boss appear.

namespace ecs {
    namespace system {

        /**
         * @class BossSpawnSystem
         * @brief System responsible for spawning boss entities.
         * The `BossSpawnSystem` manages the creation of boss entities at the end of a level.
         * It handles boss spawning logic, ensuring boss are created with the appropriate components,
         * such as position, velocity, life, and sprite information.
         */
        class BossSpawnSystem : virtual public ecs::system::ISystem {
          public:
            BossSpawnSystem() = default;
            ~BossSpawnSystem() = default;

            /**
             * @brief Executes the boss spawning system, creating new boss entities.
             * This function processes the spawning of the boss based on the conditions defined in the
             * `BossSpawn` component. It spawns a new boss when a certain score is reached.
             * @param reg The ECS registry containing entities and their components.
             */
            void operator()(Registry &reg) override;

          private:
            /**
             * @brief Creates the boss according to desired characteristic.
             * @param reg The ECS registry containing entities and their components.
             * @param types Array of types components for entities.
             * @param i Index of the boss entity.
             */
            void createNewBoss(Registry &reg, SparseArray<component::Type> &types, const std::size_t &i);

            /**
             * @brief Find the index of the chosen entity.
             * @param types Array of types components for entities.
             * @return The index of the chosen entity.
             */
            std::size_t findChosenEntity(SparseArray<component::Type> &types, const ecs::EntityType &entityToFind);

            /**
             * @brief Changes the current level depending on which boss was slayed.
             * @param reg The ECS registry containing entities and their components.
             * @param types Array of types components for entities.
             */
            void updateOngoingLevelStatus(Registry &reg, SparseArray<component::Type> &types);

            /**
             * @brief Changes the sprite for the level 2 boss.
             * @param reg The ECS registry containing entities and their components.
             * @param types Array of types components for entities.
             */
            void changeSpriteLevel2Boss(Registry &reg, SparseArray<component::Type> &types);

            /**
             * @brief Handles the logic for the spawning of the Level 3 boss.
             * @param reg The ECS registry containing entities and their components.
             * @param types Array of types components for entities.
             * @param bossEntity Index corresponding to the entity of the boss.
             */
            void spawnBossLevel3(Registry &reg, SparseArray<component::Type> &types, const std::size_t &bossEntity);

            std::size_t findBackgroundMusic(Registry &reg);
        };
    }
}

#endif /* !BOSSSPAWNSYSTEM_HPP_ */
