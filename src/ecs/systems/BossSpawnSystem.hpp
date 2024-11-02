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
             * @brief Creates the boss for level 1 according to desired characteristic.
             * @param reg The ECS registry containing entities and their components.
             * @param types Array of types components for entities.
             * @param i Index of the boss entity.
             */
            void createNewBoss(Registry &reg, SparseArray<component::Type> &types, const std::size_t &i, const float &x, const float &y, const std::size_t &level3BossIndex);

            /**
             * @brief Find the index of the Level1 Boss entity.
             * @param types Array of types components for entities.
             * @return The index of the boss' entity.
             */
            std::size_t findChosenBossEntity(SparseArray<component::Type> &types, const ecs::EntityType &entityToFind);

        };
    }
}

#endif /* !BOSSSPAWNSYSTEM_HPP_ */
