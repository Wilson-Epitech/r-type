/*
** R_TYPE PROJECT, 2024
** powerup
** File description:
** PowerupSystem
*/

#ifndef POWERUPSYSTEM_HPP_
#define POWERUPSYSTEM_HPP_

#include "Components.hpp"
#include "ISystem.hpp"
#include <raylib.h>

#define EFFECT_DURATION 5

namespace ecs {
    namespace system {

        /**
         * @class PowerupSystem
         * @brief System responsible for handling player powerup behavior.
         * The `PowerupSystem` manages the activation and the deactivation powerup. It ensures that
         * when powerup is hit by a player an effect apear and modify the comportement of the player
         */
        class PowerupSystem : virtual public ecs::system::ISystem {
            public:
                PowerupSystem() = default;
                ~PowerupSystem() = default;

                /**
                * @brief Executes the powerup system, processing player powerup.
                * This function handles the powerup logic for player entities. It determines
                * witch player's component has to be update
                * @param reg The ECS registry containing entities and their components.
                */
                void operator()(Registry &reg) override;
            private:
                /**
                * @brief summon a powerup at the death of an enemy.
                * This method generates a new powerup entity when the player hit it. It sets the position, velocity,
                * sprite information, powerup type, and other components necessary for the powerup entity.
                * @param reg The ECS registry containing entities and their components.
                * @param index The entity of the dead enemy.
                */
                void summonPowerUp(Registry &reg, const std::size_t &index);

                /**
                * @brief use a powerup when a player hit it.
                * This method change the comportement of somes player's entities
                * @param reg The ECS registry containing entities and their components.
                * @param index The entity of the dead enemy.
                * @return std::pair<std::size_t, ecs::Poweruptype> The index of the player entity and the type of the used powerup.
                */
                std::pair<std::size_t, ecs::Poweruptype> usePowerup(Registry &reg, const std::size_t &index, SparseArray<component::Life> &lives);

                /**
                * @brief Retrieves the index of the powerup component.
                * This method finds the index of the entity with the powerup template type
                * @param reg The ECS registry containing entities and components.
                * @return std::size_t The index of the powerup template entity.
                */
                std::size_t getPowerupIndex(Registry &reg);

                /**
                * @brief Clear the effect of the PowerShoot powerup
                * This method clear the effet of the powerup after a while
                * @param reg The ECS registry containing entities and their components.
                * @param powerShootTimer Vector of all player entities that has used a PowerShoot powerup
                * and the moment when it was activated
                */
                void clearPowerShootEffect(Registry &reg, std::vector<std::pair<std::size_t, std::chrono::steady_clock::time_point>> &powerShootTimer);

                /**
                * @brief Clear the effect of the Invincible powerup
                * This method clear the effet of the powerup after a while
                * @param reg The ECS registry containing entities and their components.
                * @param invincibleTimer Vector of all player entities that has used a Invincible powerup
                * and the moment when it was activated
                */
                void clearInvicibleEffect(Registry &reg, std::vector<std::pair<std::size_t, std::chrono::steady_clock::time_point>> &invincibleTimer);

                /**
                * @brief Clear the effect of the MegaShoot powerup
                * This method clear the effet of the powerup after a while
                * @param reg The ECS registry containing entities and their components.
                * @param megaShootTimer Vector of all player entities that has used a MegaShoot powerup
                * and the moment when it was activated
                */
                void clearMegaShootEffect(Registry &reg, std::vector<std::pair<std::size_t, std::chrono::steady_clock::time_point>> &megaShootTimer);

                /**
                * @brief Select ramdomely a powerup to spawn after the death of an enemy
                * @return ecs::Poweruptype The powerup type that will be spawn.
                */
                ecs::Poweruptype selectPowerup(void);

                /**
                * @brief Update the activated poweurp of the player
                * This method store the activated or deactivation powerup of a player
                * @param reg The ECS registry containing entities and their components.
                * @param playerEntity The entity of the player
                * @param powerupType The type of powerup that has been activated / deactivation
                * @param isActive Bool that inform if the powerup is activated or desactivated
                */
                void updateActivePowerup(Registry &reg, const std::size_t &playerEntity, const ecs::Poweruptype &powerupType, const bool &isActive);
        };
    }
}

#endif /* !POWERUPSYSTEM_HPP_ */
