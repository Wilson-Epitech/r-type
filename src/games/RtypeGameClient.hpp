/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** RTypeGameClient
*/

/**
 * @file RtypeGameClient.hpp
 * @brief Declares the `RtypeGameClient` class for managing the client-side game logic of R-Type.
 * This file contains the declaration of the `RtypeGameClient` class, which is responsible for setting up
 * the game's components, systems, and entities, and for managing the game state during gameplay.
 */

#ifndef RTYPEGAMECLIENT_HPP_
#define RTYPEGAMECLIENT_HPP_

#include "AGame.hpp"
#include "ControlSystem.hpp"
#include "DrawLifeBarSystem.hpp"
#include "DrawSpriteSystem.hpp"
#include "DrawTextSystem.hpp"
#include "FiringSystem.hpp"
#include "MusicSystem.hpp"
#include "PositionSystem.hpp"
#include "SoundSystem.hpp"
#include "SpriteAnimationSystem.hpp"

namespace game {

    /**
     * @class RtypeGameClient
     * @brief Manages the client-side game logic for R-Type.
     * The `RtypeGameClient` class is responsible for initializing and managing the game's components, systems,
     * and entities on the client side. It defines the game's behavior, including registering components,
     * setting up systems, spawning entities, and managing the game state.
     */
    class RtypeGameClient : virtual public game::AGame {
      public:
        /**
         * @brief Constructs the `RtypeGameClient` with the given game name.
         * This constructor initializes the game by setting up the necessary components and systems for the R-Type game.
         * @param gameName The name of the game (default is "R-Type").
         */
        RtypeGameClient(const std::string gameName = "R-Type");

        /**
         * @brief Destructor for the `RtypeGameClient` class.
         */
        ~RtypeGameClient() = default;

      private:
        /**
         * @brief Defines the current game state.
         * This method overrides the base class's `gameState()` method to manage and return the current game state.
         * @param reg The ECS registry managing entities and systems.
         * @return game::GameStatus The current game state.
         */
        GameStatus gameState(ecs::Registry &reg) override;

        /**
         * @brief Spawns the entities required for the game.
         * This method overrides the base class's `spawnEntities()` method and is responsible for creating and
         * initializing game entities such as players, enemies, and projectiles.
         */
        void spawnEntities() override;

        /**
         * @brief Registers the components used in the ECS.
         * This method registers the necessary components for entities in the ECS, such as position, velocity, drawing,
         * and sound components.
         */
        void registerComponents() override;

        /**
         * @brief Adds components to the entities.
         * This method assigns the registered components to the respective game entities.
         */
        void addComponentsToEntities() override;

        /**
         * @brief Adds systems to the ECS registry.
         * This method adds the necessary systems (e.g., control, drawing, animation, sound) to the ECS registry to
         * manage the game's logic.
         */
        void addSystemsToRegistry() override;
    };
}

#endif /* !RTYPEGAMECLIENT_HPP_ */
