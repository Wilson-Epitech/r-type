/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** KillOffscreenEntitiesSystem
*/

/**
* @file KillOffscreenEntitiesSystem.hpp
* @brief Declares the system responsible for removing entities that go off-screen in the game engine ECS.
* This file contains the `KillOffscreenEntitiesSystem` class, which handles the logic for removing entities
* that leave the visible game screen. It ensures that entities such as projectiles and enemies are removed
* when they move outside the boundaries of the game view.
*/

#ifndef KILLOFFSCREENENTITIESSYSTEM_HPP_
#define KILLOFFSCREENENTITIESSYSTEM_HPP_

#include "Components.hpp"
#include "ISystem.hpp"

namespace ecs {
    namespace system {

        /**
        * @class KillOffscreenEntitiesSystem
        * @brief System responsible for removing off-screen entities.
        * The `KillOffscreenEntitiesSystem` detects when entities leave the visible game screen
        * and removes them from the game. It is commonly used for entities such as player or enemy shots,
        * enemies, and other objects that should be destroyed once they move outside the viewable area.
        */
        class KillOffscreenEntitiesSystem : virtual public ecs::system::ISystem {
          public:
            KillOffscreenEntitiesSystem() = default;
            ~KillOffscreenEntitiesSystem() = default;

            /**
            * @brief Executes the system to remove entities that have gone off-screen.
            * This function checks the position of all relevant entities and removes any that
            * have moved beyond the boundaries of the game screen. This helps in managing performance
            * and preventing inactive entities from persisting unnecessarily.
            * @param reg The ECS registry containing entities and their components.
            */
            void operator()(Registry &reg) override;
        };
    }
}

#endif /* !KILLOFFSCREENENTITIESSYSTEM_HPP_ */
