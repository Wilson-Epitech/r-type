/*
** R_TYPE PROJECT, 2024
** enemy
** File description:
** EnemySpawnSystem
*/

/**
 * @file EnemySpawnSystem.hpp
 * @brief Declares the system responsible for spawning enemy entities in the game engine ECS.
 * This file contains the `EnemySpawnSystem` class, which handles the logic of spawning enemies in waves
 * at specific intervals or under certain conditions.
 */

#ifndef ENEMYSPAWNSYSTEM_HPP_
#define ENEMYSPAWNSYSTEM_HPP_

#include "Components.hpp"
#include "ISystem.hpp"

namespace ecs {
    namespace system {

        /**
         * @class EnemySpawnSystem
         * @brief System responsible for spawning enemy entities.
         * The `EnemySpawnSystem` manages the creation of enemy entities at designated intervals or waves.
         * It handles enemy spawning logic, ensuring enemies are created with the appropriate components,
         * such as position, velocity, life, and sprite information.
         */
        class EnemySpawnSystem : virtual public ecs::system::ISystem {
          public:
            EnemySpawnSystem() = default;
            ~EnemySpawnSystem() = default;

            /**
             * @brief Executes the enemy spawning system, creating new enemy entities.
             * This function processes the spawning of enemies based on the conditions defined in the
             * `EnemySpawn` component. It spawns new enemies at set intervals or as part of enemy waves.
             * @param reg The ECS registry containing entities and their components.
             */
            void operator()(Registry &reg) override;

          private:
            /**
             * @brief Creates a new enemy entity during a spawn event.
             * This method handles the creation of a new enemy, setting up the necessary components
             * such as position, velocity, life, and sprite information.
             * @param reg The ECS registry where the new enemy entity will be created.
             * @param enemySpawn Array of `EnemySpawn` components containing spawn information.
             * @param velocities Array of velocity components for entities.
             * @param lives Array of life components for entities.
             * @param spriteInfos Array of sprite information components for entities.
             * @param i The index of the spawning enemy.
             */
            void createNewEnemy(Registry &reg, SparseArray<component::EnemySpawn> &enemySpawn, SparseArray<component::Velocity> &velocities,
                                SparseArray<component::Life> &lives, SparseArray<component::SpriteInfos> &spriteInfos,
                                const std::size_t &i);

            /**
             * @brief Loop through the entity and return the current score.
             * @param reg The ECS registry containing entities and their components.
             * @return Returns the currenct score of the game.
             */
            std::size_t checkCurrentScore(Registry &reg);
        };
    }
}

#endif /* !ENEMYSPAWNSYSTEM_HPP_ */
