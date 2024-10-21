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

namespace ecs {
    namespace system {
        class PowerupSystem : virtual public ecs::system::ISystem {
            public:
                PowerupSystem() = default;
                ~PowerupSystem() = default;

                void operator()(Registry &reg) override;
            private:
                void summonPowerUp(Registry &reg, const std::size_t index);
                void usePowerup(Registry &reg, const std::size_t index);
                std::size_t getPowerupIndex(Registry &reg);
        };
    }
}

#endif /* !POWERUPSYSTEM_HPP_ */
