/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** LifeSystem
*/

/**
* @file LifeSystem.hpp
* @brief Declares the system responsible for managing the life of entities in the game engine ECS.
* This file contains the `LifeSystem` class, which handles the logic for managing entities' life components.
* It ensures that entities are removed from the game when their life reaches zero.
*/

#ifndef LIFESYSTEM_HPP_
#define LIFESYSTEM_HPP_

#include "Components.hpp"
#include "ISystem.hpp"
#include <raylib.h>

namespace ecs {
    namespace system {

        /**
        * @class LifeSystem
        * @brief System responsible for managing entity life and removing dead entities.
        * The `LifeSystem` monitors the life components of entities and removes those whose
        * life has reached zero. It ensures that entities are properly removed from the game
        * when they are considered "dead."
        */
        class LifeSystem : virtual public ecs::system::ISystem {
          public:
            LifeSystem() = default;
            ~LifeSystem() = default;

            /**
            * @brief Executes the life management system, removing dead entities.
            * This function checks the life components of all entities and removes any entity
            * whose life has dropped to zero. It helps maintain the game state by cleaning up
            * entities that are no longer active.
            * @param reg The ECS registry containing entities and their components.
            */
            void operator()(Registry &reg) override;
        };
    }
}

#endif /* !LIFESYSTEM_HPP_ */
