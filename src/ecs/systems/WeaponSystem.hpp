/*
** R_TYPE PROJECT, 2024
** mirror_playerWeapon
** File description:
** WeaponSystem
*/

#ifndef WEAPONSYSTEM_HPP_
#define WEAPONSYSTEM_HPP_

#include "Components.hpp"
#include "ISystem.hpp"

namespace ecs {
    namespace system {

        /**
         * @class WeaponSystem
         * @brief System responsible for handling player weapon behavior.
         * The `WeaponSystem` manages the activation and the deactivation weapon. It ensures that
         * when weapon is hit by a player an effect apear and modify the comportement of the player
         */
        class WeaponSystem : virtual public ecs::system::ISystem {
          public:
            WeaponSystem() = default;
            ~WeaponSystem() = default;

            /**
             * @brief Executes the weapon system, processing player weapon.
             * This function handles the weapon logic for player entities. It determines
             * witch player's component has to be update
             * @param reg The ECS registry containing entities and their components.
             */
            void operator()(Registry &reg) override;

          private:
            /**
             * @brief summon a weapon at the death of an enemy.
             * This method generates a new weapon entity when the player hit it. It sets the position, velocity,
             * sprite information, weapon type, and other components necessary for the weapon entity.
             * @param reg The ECS registry containing entities and their components.
             * @param index The entity of the dead enemy.
             */
            void summonWeapon(Registry &reg, const std::size_t &index);

            /**
             * @brief Retrieves the index of the weapon component.
             * This method finds the index of the entity with the weapon template type
             * @param reg The ECS registry containing entities and components.
             * @return std::size_t The index of the weapon template entity.
             */
            std::size_t getWeaponIndex(Registry &reg);

            /**
             * @brief Retrieves the index of the weapon sprite component.
             * This method finds the index of the entity with the weapon sprite type
             * @param reg The ECS registry containing entities and components.
             * @param weaponType The type of weapon sprite to retrieves.
             * @return std::size_t The index of the weapon sprite entity.
             */
            std::size_t getWeaponSpriteIndex(Registry &reg, ecs::WeaponType weaponType);

            /**
             * @brief Activate the weapon that a player took.
             * @param reg The ECS registry containing entities and components.
             * @param index The type of the weapon to activate.
             */
            void activateWeapon(Registry &reg, const std::size_t &index);

            /**
             * @brief Deactivate the weapon that a player took.
             * This method desactivate a weapon after a while
             * @param reg The ECS registry containing entities and components.
             * @param index The type of the weapon to desactivate.
             */
            void desactivateWeapon(Registry &reg, const std::size_t &index);
        };
    }
}

#endif /* !WEAPONSYSTEM_HPP_ */
