/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** PlayerSpawnSystem
*/

#ifndef PLAYERSPAWNSYSTEM_HPP_
#define PLAYERSPAWNSYSTEM_HPP_

#include "Components.hpp"
#include "ISystem.hpp"

namespace ecs {
    namespace system {
        class PlayerSpawnSystem : virtual public ecs::system::ISystem {
          public:
            PlayerSpawnSystem() = default;
            ~PlayerSpawnSystem() = default;

            void operator()(Registry &reg) override;

          private:
            void createNewPlayer(Registry &reg, entity_t entity);
            std::size_t getPlayerTemplateIndex(Registry &reg);
        };
    }
}

#endif /* !PLAYERSPAWNSYSTEM_HPP_ */
