/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** FiringSystem
*/

/**
 * @file FiringSystem.hpp
 * @brief Declares the system responsible for handling player firing in the game engine ECS.
 * This file contains the `FiringSystem` class, which manages the logic for player entities firing projectiles.
 * It handles the creation of shots when the player fires and ensures that firing behavior is processed correctly.
 */

#ifndef FIRINGSYSTEM_HPP_
#define FIRINGSYSTEM_HPP_

#include "Assets.hpp"
#include "Components.hpp"
#include "ISystem.hpp"

namespace ecs {
    namespace system {

        /**
         * @class FiringSystem
         * @brief System responsible for handling player firing behavior.
         * The `FiringSystem` manages the creation of shots fired by player entities. It ensures that
         * when the player fires, a corresponding shot entity is created with the necessary components,
         * such as position, velocity, and sprite information.
         */
        class FiringSystem : virtual public ecs::system::ISystem {
          public:
            FiringSystem() = default;
            ~FiringSystem() = default;

            /**
             * @brief Executes the firing system, processing player shots.
             * This function handles the firing logic for player entities. It determines when the player
             * fires and creates the corresponding shot entities with appropriate components.
             * @param reg The ECS registry containing entities and their components.
             */
            void operator()(Registry &reg) override;

          private:
            /**
             * @brief Creates a shot fired by a player entity.
             * This method generates a new shot entity when the player fires. It sets the position, velocity,
             * sprite information, and other components necessary for the shot entity.
             * @param index The index of the player entity firing the shot.
             * @param reg The ECS registry where the shot entity will be created.
             * @param shootPower The power of the shoot.
             * @param shootSize The size of the shoot.
             * @param weaponType The type of weapon that shot.
             */
            void addShot(const std::size_t &index, Registry &reg, const std::size_t &shootPower, const float &shootSize, const ecs::WeaponType &weaponType);

            /**
             * @brief Retrieves the index of the player shot component.
             * This method finds the index of the entity with the shooting component that is responsible
             * for handling the player's shots.
             * @param reg The ECS registry containing entities and components.
             * @return std::size_t The index of the player entity with a shooting component.
             */
            std::size_t getPlayerShootIndex(Registry &reg);

            /**
             * @brief Creates multiple shots for a player entity based on available weapons.
             * This method iterates through the player's weapons in the ECS registry and creates a shot
             * entity for each weapon the player possesses. Each shot's position, velocity, sprite information,
             * and other properties are set according to the weapon's type and the specified power and size.
             * If the player has no weapons, a default shot is created.
             * @param reg The ECS registry containing entities and components.
             * @param playerIndex The index of the player entity that is firing shots.
             * @param shootPower The power of each shot.
             * @param shootSize The size of each shot.
             */
            void addEveryShoot(Registry &reg, const std::size_t &playerIndex, const std::size_t &shootPower, const float &shootSize);
        };
    }
}

#endif /* !FIRINGSYSTEM_HPP_ */
