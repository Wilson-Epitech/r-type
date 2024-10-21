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
        class PowerupSystem : virtual public ecs::system::ISystem {
            public:
                PowerupSystem() = default;
                ~PowerupSystem() = default;

                void operator()(Registry &reg) override;
            private:
                void summonPowerUp(Registry &reg, const std::size_t index);
                std::pair<std::size_t, ecs::Poweruptype> usePowerup(Registry &reg, const std::size_t index);
                std::size_t getPowerupIndex(Registry &reg);
                void clearPowerShootEffect(Registry &reg, std::vector<std::pair<std::size_t, std::chrono::steady_clock::time_point>> &powerShootTimer);
                void clearInvicibleEffect(Registry &reg, std::vector<std::pair<std::size_t, std::chrono::steady_clock::time_point>> &invincibleTimer);
                ecs::Poweruptype selectPowerup(void);
        };
    }
}

#endif /* !POWERUPSYSTEM_HPP_ */
