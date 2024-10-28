/*
** R_TYPE PROJECT, 2024
** branch
** File description:
** MusicSystem
*/

/**
 * @file MusicSystem.hpp
 * @brief Declares the system responsible for managing background music in the game engine ECS.
 * This file contains the `MusicSystem` class, which handles the loading and playback of background music
 * for the game. It ensures that background music is loaded and played appropriately based on game conditions.
 */

#ifndef MUSICSYSTEM_HPP_
#define MUSICSYSTEM_HPP_

#include "Components.hpp"
#include "ISystem.hpp"
#include "RaylibComponents.hpp"

namespace ecs {
    namespace system {

        /**
         * @class MusicSystem
         * @brief System responsible for handling background music playback.
         * The `MusicSystem` manages the loading and playback of background music in the game.
         * It ensures that music files are loaded when necessary and played according to the game state.
         */
        class MusicSystem : virtual public ecs::system::ISystem {
          public:
            MusicSystem() = default;
            ~MusicSystem() = default;

            /**
             * @brief Executes the music system to handle background music.
             * This function checks for music components within entities and ensures that music is
             * loaded and played. It processes any logic related to the music playback in the game.
             * @param reg The ECS registry containing entities and their components.
             */
            void operator()(Registry &reg) override;

          private:
            /**
             * @brief Loads the background music for an entity if not already loaded.
             * This method checks if the music for an entity is loaded, and if not, it loads the music in the `BackgroundMusicObject`
             * component from the specified file path from the `BackgroundMusicInfos` component.
             * @param backgroundMusicInfos Array of background music infos components for entities.
             * @param backgroundMusicObject Array of background music objects components for entities.
             */
            void loadMusics(SparseArray<component::BackgroundMusicInfos> &backgroundMusicInfos,
                            SparseArray<component::BackgroundMusicObject> &backgroundMusicObjects);
        };
    }
}

#endif /* !MUSICSYSTEM_HPP_ */
