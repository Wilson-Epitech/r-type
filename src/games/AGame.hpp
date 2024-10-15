/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** AGame
*/

/**
 * @file AGame.hpp
 * @brief Defines the abstract base class `AGame` for managing common game logic and structure.
 * This file contains the declaration of the `AGame` class, which serves as a base class for specific games,
 * providing a common structure for initializing and running a game. It includes methods for entity and component
 * management and system registration.
 */

#ifndef AGAME_HPP_
#define AGAME_HPP_

#include "Components.hpp"
#include "IGame.hpp"

namespace game {

    /**
     * @class AGame
     * @brief Abstract base class providing the structure for game management.
     * The `AGame` class defines the common logic required for game initialization and execution.
     * Derived classes are expected to implement specific game logic using this foundation.
     */
    class AGame : virtual public game::IGame {
      public:
        /**
         * @brief Constructs the `AGame` class with the given game name.
         * @param gameName The name of the game.
         */
        AGame(const std::string &gameName) : _gameName(gameName){};
        ~AGame() = default;

        /**
         * @brief Executes the game logic.
         * This method is called to update the game state using the ECS registry.
         * @param reg The ECS registry containing entities and components.
         * @return The current game status.
         */
        GameStatus operator()(ecs::Registry &reg) override {
            return gameState(reg);
        }

        /**
         * @brief Returns a reference to the ECS registry.
         * This method provides access to the ECS registry, which manages the entities and components.
         * @return A reference to the `ecs::Registry` object.
         */
        ecs::Registry &getRegistry() override {
            return _reg;
        }

        /**
         * @brief Returns the name of the game.
         * This method provides access to the game's name.
         * @return A reference to the game name as a `std::string`.
         */
        std::string &getGameName() override {
            return _gameName;
        }

      protected:
        /**
         * @brief Initializes the game by setting up entities, components, and systems.
         * This method is called to initialize the game by spawning entities, registering components, adding
         * components to entities, and registering the necessary systems in the ECS registry.
         */
        void initializeGame() override {
            spawnEntities();
            registerComponents();
            addComponentsToEntities();
            addSystemsToRegistry();
        };

        ecs::Registry _reg;                                        ///< The ECS registry managing the entities and components.
        std::string _gameName;                                     ///< The name of the game.
        std::unordered_map<std::string, entity_t> _stringToEntity; ///< A mapping from string identifiers to entity IDs.
    };
}

#endif /* !AGAME_HPP_ */
