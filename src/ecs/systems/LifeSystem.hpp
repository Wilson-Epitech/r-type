/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** LifeSystem
*/

#ifndef LIFESYSTEM_HPP_
#define LIFESYSTEM_HPP_

#include "Components.hpp"
#include "ISystem.hpp"
#include <raylib.h>

namespace ecs {
    namespace system {
        class LifeSystem : virtual public ecs::system::ISystem {
          public:
            LifeSystem() = default;
            ~LifeSystem() = default;

            void operator()(Registry &reg) override;
        };
    }
}

#endif /* !LIFESYSTEM_HPP_ */
