/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** ISystem
*/

/**
 * @file ISystem.hpp
 * @brief Declares the interface for all systems in the game engine ECS.
 * This file defines the `ISystem` interface, which is implemented by all systems in the ECS.
 * The interface ensures that each system has a method for processing entities within the ECS registry.
 */

#ifndef ISYSTEM_HPP_
#define ISYSTEM_HPP_

#include "Lib.hpp"
#include "Registry.hpp"
#include <chrono>

#define SCORE_FINAL_BOSS_LEVEL1 3000

namespace ecs {
    namespace system {

        /**
         * @class ISystem
         * @brief Interface for all systems in the ECS architecture.
         * The `ISystem` class defines the base interface for all ECS systems. Each system
         * that derives from `ISystem` must implement the `operator()` method, which is used
         * to process entities in the ECS registry.
         */
        class ISystem {
          public:
            ISystem() = default;
            ~ISystem() = default;

            /**
             * @brief Processes entities in the ECS registry.
             * This pure virtual method is implemented by each system to process entities
             * and their components within the ECS registry. The specific logic depends on
             * the system implementation.
             * @param reg The ECS registry containing entities and components.
             */
            virtual void operator()(Registry &reg) = 0;
        };
    }
}

#endif /* !ISYSTEM_HPP_ */
