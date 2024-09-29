/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** KillOffscreenEntitiesSystem
*/

#ifndef KILLOFFSCREENENTITIESSYSTEM_HPP_
#define KILLOFFSCREENENTITIESSYSTEM_HPP_

#include "Components.hpp"
#include "ISystem.hpp"

namespace ecs {
    namespace system {
        class KillOffscreenEntitiesSystem : virtual public ecs::system::ISystem {
          public:
            KillOffscreenEntitiesSystem() = default;
            ~KillOffscreenEntitiesSystem() = default;

            void operator()(Registry &reg) override;
        };
    }
}

#endif /* !KILLOFFSCREENENTITIESSYSTEM_HPP_ */
