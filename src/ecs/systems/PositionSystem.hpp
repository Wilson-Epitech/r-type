/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** PositionSystem
*/

/**
 * @file PositionSystem.hpp
 * @brief Declares the system responsible for updating the position of entities in the game engine ECS.
 * This file contains the `PositionSystem` class, which handles updating the position of entities based
 * on their velocity. It ensures that entities move correctly within the game world by adjusting their positions.
 */

#ifndef POSITIONSYSTEM_HPP_
#define POSITIONSYSTEM_HPP_

#include "Components.hpp"
#include "ISystem.hpp"

namespace ecs {
    namespace system {

        /**
         * @class PositionSystem
         * @brief System responsible for updating entity positions based on their velocity.
         * The `PositionSystem` adjusts the position of each entity in the game based on its velocity component.
         * It ensures that entities are moved correctly, and in the case of player-controlled entities,
         * it includes additional logic for handling player movement constraints.
         */
        class PositionSystem : virtual public ecs::system::ISystem {
          public:
            PositionSystem() = default;
            ~PositionSystem() = default;

            /**
             * @brief Executes the position update system.
             * This function processes the position and velocity components of entities, updating
             * their positions accordingly. It applies logic for both general entities and player-controlled entities.
             * @param reg The ECS registry containing entities and their components.
             */
            void operator()(Registry &reg) override;

          private:
            /**
             * @brief Handles the position update for player-controlled entities.
             * This method ensures that the player entities' positions are updated correctly based on their velocity
             * while considering additional constraints, such as staying within the screen bounds.
             * @param playerIndex The index of the player entity.
             * @param reg The ECS registry containing entities and their components.
             * @return true If the player's position was updated successfully.
             * @return false If the player's position update was not necessary.
             */
            bool handlePlayerPosition(const std::size_t &playerIndex, Registry &reg);

            /**
             * @brief Store the position of the entity before any movement
             * @param reg The ECS registry containing entities and their components.
             * @param index The index of the entity to store the position.
             */
            void setStartPosition(Registry &reg, const std::size_t &index);

            /**
             * @brief Handle the movement pattern of the entity
             * @param reg The ECS registry containing entities and their components.
             * @param index The entity that will move.
             */
            void handleMovementPattern(Registry &reg, const std::size_t &index);

            /**
             * @brief Handles the position update for an entity.
             * This method move the entity with a straight movement
             * @param reg The ECS registry containing entities and their components.
             * @param index The index of the entity to move.
             */
            void moveStraight(Registry &reg, const std::size_t &index);

            /**
             * @brief Handles the position update for an entity.
             * This method move the entity with a snake movement
             * @param reg The ECS registry containing entities and their components.
             * @param index The index of the entity to move.
             */
            void moveSnake(Registry &reg, const std::size_t &index);

            /**
             * @brief Handles the position update for an entity.
             * This method move the entity in the direction of the player
             * @param reg The ECS registry containing entities and their components.
             * @param index The index of the entity to move.
             */
            void moveFollow(Registry &reg, const std::size_t &index);

            /**
             * @brief Handles the velocity update for an entity.
             * This method reverse the velocity of an entity if it go out of the screen
             * @param reg The ECS registry containing entities and their components.
             * @param index The index of the entity to move.
             */
            void handleReverseDirection(Registry &reg, const std::size_t &index);

            std::size_t findClosestPlayer(Registry &reg, const std::size_t &index);
        };
    }
}

#endif /* !POSITIONSYSTEM_HPP_ */
