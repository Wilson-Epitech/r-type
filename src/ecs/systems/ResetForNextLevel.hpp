/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** ResetForNextLevel
*/

/**
 * @file ResetForNextLevel.hpp
 * @brief Declares the system responsible for creating a new level.
 * This file contains the `ResetForNextLevel` class, which handles the logic of resetting the game
 * when a certain score is hit to create a new level.
 */

#ifndef RESETFORNEXTLEVEL_HPP_
#define RESETFORNEXTLEVEL_HPP_

#include "Components.hpp"
#include "ISystem.hpp"

#define LEVEL2_BACKGROUND_PATH "assets/background_level2.png" ///< Path for the second level's background.
#define RESET_POS_PLAYER_X 100; ///< Reset the x position of the players for new level.
#define RESET_POS_PLAYER_Y 400; ///< Reset the y position of the players for new level.

namespace ecs {
    namespace system {

        /**
         * @class ResetForNextLevel
         * @brief System responsible for resetting the world and creating new level.
         * The `ResetForNextLevel` manages the resetting and creation of a new level.
         * It handles the reseting of all the players' positions, killing the remaining enemies
         * after the death of the boss, the new background sprite.
         */
        class ResetForNextLevel : virtual public ecs::system::ISystem {
            public:
                ResetForNextLevel() = default;
                ~ResetForNextLevel() = default;

                /**
                 * @brief Executes the reseting of the current level, creating the new level.
                * This function processes the reseting of the current level and the creation
                * of the new level if the boss of preceding level was beaten.
                * It erase all remaining enemies, reset the players' positions and set the new background.
                * @param reg The ECS registry containing entities and their components.
                */
                void operator()(Registry &reg) override;

            protected:
            private:
                /**
                * @brief Loop through the players to reset their positions and velocity.
                * @param reg The ECS registry containing entities and their components.
                * @param types Array of types components for entities.
                */
                void resetPlayerPosition(Registry &reg, SparseArray<component::Type> &types);

                /**
                * @brief Loop through the remaining enemies to kill them.
                * @param reg The ECS registry containing entities and their components.
                * @param types Array of types components for entities.
                */
                void killAllRemainingEnemies(Registry &reg, SparseArray<component::Type> &types);

                /**
                * @brief Changes the path of the background sprite used.
                * @param reg The ECS registry containing entities and their components.
                * @param types Array of types components for entities.
                */
                void setNewLevelBackground(Registry &reg, SparseArray<component::Type> &types);
        };
    }
}

#endif /* !RESETFORNEXTLEVEL_HPP_ */
