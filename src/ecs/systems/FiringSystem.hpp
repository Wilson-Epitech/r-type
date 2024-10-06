/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** FiringSystem
*/

#ifndef FIRINGSYSTEM_HPP_
#define FIRINGSYSTEM_HPP_

#include "Assets.hpp"
#include "Components.hpp"
#include "ISystem.hpp"

#define SHOOT_RATE 0.4

namespace ecs {
    namespace system {
        class FiringSystem : virtual public ecs::system::ISystem {
          public:
            FiringSystem() = default;
            ~FiringSystem() = default;

            void operator()(Registry &reg) override;

          private:
            void addShot(std::size_t index, Registry &reg);
            std::size_t getPlayerShootIndex(Registry &reg);
        };
    }
}

#endif /* !FIRINGSYSTEM_HPP_ */
