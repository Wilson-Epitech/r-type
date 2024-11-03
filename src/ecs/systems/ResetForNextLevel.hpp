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

#include "Assets.hpp"
#include "Components.hpp"
#include "ISystem.hpp"

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
             * @param pathToSprite Path to the new background sprite.
             */
            void setNewLevelBackground(Registry &reg, SparseArray<component::Type> &types, const std::string &pathToSprite);

            /**
             * @brief Changes the path of the ennemy sprite used.
             * @param reg The ECS registry containing entities and their components.
             * @param types Array of types components for entities.
             */
            void changeSpriteForEnemies(Registry &reg, SparseArray<component::Type> &types);

            /**
             * @brief Find the index of the Level1 Boss entity.
             * @param types Array of types components for entities.
             * @return The index of the boss' entity.
             */
            std::size_t findLevel1BossEntity(Registry &reg);

            /**
             * @brief Find the index of the background music entity.
             * @param reg The ECS registry containing entities and their components.
             * @return The index of the background music.
             */
            std::size_t findBackgroundMusic(Registry &reg, const ecs::CurrentLevel &level);

            /**
             * @brief Find the index of the background music entity that currently play music.
             * @param reg The ECS registry containing entities and their components.
             * @return The index of the background music that currently play music.
             */
            std::size_t findCurrentBackgroundMusic(Registry &reg);

            void changeMusic(Registry &reg, const ecs::CurrentLevel &level);
        };
    }
}

#endif /* !RESETFORNEXTLEVEL_HPP_ */
