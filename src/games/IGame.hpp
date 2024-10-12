/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** IGame
*/

/**
* @file IGame.hpp
* @brief Defines the `IGame` interface for managing the game structure.
* This file declares the `IGame` interface, which outlines the necessary methods that any game class
* should implement to manage game state, entities, components, and systems.
*/

#ifndef IGAME_HPP_
#define IGAME_HPP_

#include "Registry.hpp"

namespace game {

    /**
    * @enum GameStatus
    * @brief Represents the different states of the game.
    * This enum is used to indicate the current status of the game, whether the game is still playing, won, or lost.
    */
    enum GameStatus {
        PLAYING,  ///< The game is in progress.
        WON,      ///< The player has won the game.
        LOOSE     ///< The player has lost the game.
    };

    /**
    * @class IGame
    * @brief Interface defining the structure of a game class.
    * The `IGame` class provides an abstract interface for managing the essential parts of a game. Any game class
    * that implements this interface must define methods for handling game states, entity management, component
    * registration, and system management.
    */
    class IGame {
      public:
        IGame() = default;
        ~IGame() = default;

        /**
        * @brief Executes the game logic and updates the game state.
        * This method is responsible for running the game loop and updating the game state using the ECS registry.
        * @param reg The ECS registry that contains all game entities and components.
        * @return The current game status (playing, won, or lost).
        */
        virtual GameStatus operator()(ecs::Registry &reg) = 0;

        /**
        * @brief Returns a reference to the ECS registry.
        * This method provides access to the ECS registry, which manages all the entities and components in the game.
        * @return A reference to the `ecs::Registry` object.
        */
        virtual ecs::Registry &getRegistry() = 0;

        /**
        * @brief Returns the name of the game.
        * This method provides access to the game's name.
        * @return A reference to the game name as a `std::string`.
        */
        virtual std::string &getGameName() = 0;

      protected:
        /**
        * @brief Manages the current game state.
        * This method is responsible for updating and managing the game state during gameplay.
        * @param reg The ECS registry containing entities and components.
        * @return The current game status.
        */
        virtual GameStatus gameState(ecs::Registry &reg) = 0;

        /**
        * @brief Initializes the game by setting up entities, components, and systems.
        * This method is responsible for setting up the game, including spawning entities, registering components,
        * and adding systems to the ECS registry.
        */
        virtual void initializeGame() = 0;

        /**
        * @brief Spawns the necessary game entities.
        * This method is responsible for creating the entities required for the game.
        */
        virtual void spawnEntities() = 0;

        /**
        * @brief Registers components for game entities.
        * This method registers all the necessary components that will be used by game entities.
        */
        virtual void registerComponents() = 0;

        /**
        * @brief Adds components to entities.
        * This method assigns the registered components to the corresponding entities.
        */
        virtual void addComponentsToEntities() = 0;

        /**
        * @brief Adds the systems that manage the game logic.
        * This method registers the systems that control various aspects of the game, such as movement,
        * collision, and rendering.
        */
        virtual void addSystemsToRegistry() = 0;
    };
}

#endif /* !IGAME_HPP_ */
