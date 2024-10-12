/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** PlayerSpawnSystem
*/

/**
* @file PlayerSpawnSystem.hpp
* @brief Declares the system responsible for spawning player entities in the game engine ECS.
* This file contains the `PlayerSpawnSystem` class, which manages the spawning of player entities.
* It handles player creation, assigning player colors, and ensuring that players are spawned in the game
* with the necessary components.
*/

#ifndef PLAYERSPAWNSYSTEM_HPP_
#define PLAYERSPAWNSYSTEM_HPP_

#include "Components.hpp"
#include "ISystem.hpp"

#define CYAN CLITERAL(Color){120, 200, 225, 255} ///< Default color for a player

namespace ecs {
    namespace system {

        /**
        * @class PlayerSpawnSystem
        * @brief System responsible for spawning player entities.
        * The `PlayerSpawnSystem` manages the creation and setup of player entities in the game. 
        * It handles the spawning of players, assigning them appropriate components such as position, velocity, 
        * sprite, and color. It ensures that each player is properly initialized in the game.
        */
        class PlayerSpawnSystem : virtual public ecs::system::ISystem {
          public:
            PlayerSpawnSystem() = default;
            ~PlayerSpawnSystem() = default;

            /**
            * @brief Executes the player spawn system, creating new players if needed.
            * This function checks for conditions to spawn new players, creates player entities, 
            * and sets up the necessary components for each player, such as position, sprite, and color.
            * @param reg The ECS registry containing entities and their components.
            */
            void operator()(Registry &reg) override;

          private:

            /**
            * @brief Creates a new player entity.
            * This method sets up a new player entity, assigning necessary components like position, 
            * velocity, and sprite information. It also ensures that the player is initialized correctly 
            * with a unique color based on the client index.
            * @param reg The ECS registry where the player entity will be created.
            * @param entity The entity identifier for the new player.
            * @param clientIndex The index of the client, used to assign a unique color to the player.
            */
            void createNewPlayer(Registry &reg, entity_t entity, std::size_t clientIndex);

            /**
            * @brief Retrieves the index of the player template entity.
            * This method finds the index of the player template, which is used to create new player entities.
            * @param reg The ECS registry containing entities and components.
            * @return std::size_t The index of the player template entity.
            */
            std::size_t getPlayerTemplateIndex(Registry &reg);

            /**
            * @brief Sets the color for a player entity.
            * This method assigns a color to the player based on the client index, ensuring each player
            * has a distinct color.
            * @param playerEntity The entity identifier for the player.
            * @param clientIndex The index of the client, used to assign the player's color.
            * @param reg The ECS registry containing entities and components.
            */
            void setPlayerColor(entity_t playerEntity, std::size_t clientIndex, Registry &reg);
        };
    }
}

#endif /* !PLAYERSPAWNSYSTEM_HPP_ */
