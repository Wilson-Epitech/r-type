/*
** R_TYPE PROJECT, 2024
** branch
** File description:
** EnemyFiringSystem
*/

/**
 * @file EnemyFiringSystem.hpp
 * @brief Declares the system responsible for handling enemy firing in the game engine ECS.
 * This file contains the `EnemyFiringSystem` class, which manages the logic of enemy entities firing projectiles.
 * It handles shot creation and ensures the enemy's shooting behavior is correctly processed.
 */

#ifndef ENEMYFIRINGSYSTEM_HPP_
#define ENEMYFIRINGSYSTEM_HPP_

#include "Assets.hpp"
#include "Components.hpp"
#include "ISystem.hpp"
#include <math.h>

#define PI_VALUE 3.14 ///< Value of the mathematical constant pi

namespace ecs {
    namespace system {

        /**
         * @class EnemyFiringSystem
         * @brief System responsible for handling enemy firing behavior.
         * The `EnemyFiringSystem` is responsible for managing the creation of shots fired by enemy entities.
         * It processes enemy firing logic, ensuring that enemies shoot at the appropriate time intervals.
         */
        class EnemyFiringSystem : virtual public ecs::system::ISystem {
          public:
            EnemyFiringSystem() = default;
            ~EnemyFiringSystem() = default;

            /**
             * @brief Executes the enemy firing system, processing enemy shots.
             * This function handles the firing logic for enemy entities. It determines when enemies
             * should fire and creates the corresponding shot entities.
             * @param reg The ECS registry containing entities and their components.
             */
            void operator()(Registry &reg) override;

          private:
            /**
             * @brief Creates a shot fired by an enemy entity.
             * This method generates a new shot entity for an enemy when the firing conditions are met.
             * It sets the position, velocity, and other components of the shot.
             * @param index The index of the enemy entity firing the shot.
             * @param reg The ECS registry where the shot entity will be created.
             */
            void addShot(const std::size_t &index, Registry &reg);

            /**
             * @brief Finds the index of an enemy shot component.
             * This method retrieves the index of the entity that has a shooting component, used to manage enemy shots.
             * @param reg The ECS registry containing entities and components.
             * @return std::size_t The index of the enemy entity with a shooting component.
             */
            std::size_t getEnemyShootIndex(Registry &reg);

            /**
             * @brief Change the velocity of the enemy shoot.
             * This method change the velocity of the enemy shoot if the enemy pattern is 'FOLLOW'
             * The shoot will go in the direction of the player
             * @param reg The ECS registry containing entities and components.
             * @param enemyIndex The index of the enemy entity firing the shot.
             * @param shootIndex The index of the enemy shoot entity.
             * @return float the angle of the shoot.
             */
            float changeShootVelocity(Registry &reg, const std::size_t &enemyIndex, const std::size_t &shootIndex);

            /**
             * @brief Finds enemy's nearest player.
             * @param reg The ECS registry containing entities and components.
             * @param index The index of the enemy entity.
             * @return std::size_t The index of the enemy's nearest player.
             */
            std::size_t findClosestPlayer(Registry &reg, const std::size_t &index);
        };
    }
}

#endif /* !ENEMYFIRINGSYSTEM_HPP_ */
