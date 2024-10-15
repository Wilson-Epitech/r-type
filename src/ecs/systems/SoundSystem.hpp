/*
** R_TYPE PROJECT, 2024
** enemy_wave
** File description:
** SoundSystem
*/

/**
 * @file SoundSystem.hpp
 * @brief Declares the system responsible for handling sound effects in the game engine ECS.
 * This file contains the `SoundSystem` class, which manages the loading and playback of sound effects
 * for the game. It ensures that sound effects are correctly loaded and played for entities when necessary.
 */

#ifndef SOUNDSYSTEM_HPP_
#define SOUNDSYSTEM_HPP_

#include "Components.hpp"
#include "ISystem.hpp"

namespace ecs {
    namespace system {

        /**
         * @class SoundSystem
         * @brief System responsible for managing sound effects in the game.
         * The `SoundSystem` handles the loading and playback of sound effects. It ensures that sound effects
         * are triggered and played in response to game events, such as collisions, actions, or other interactions.
         */
        class SoundSystem : virtual public ecs::system::ISystem {
          public:
            SoundSystem() = default;
            ~SoundSystem() = default;

            /**
             * @brief Executes the sound system to handle sound effects.
             * This function checks for sound effect components within entities and ensures that sounds
             * are loaded and played at the appropriate moments during the game.
             * @param reg The ECS registry containing entities and their components.
             */
            void operator()(Registry &reg) override;

          private:
            /**
             * @brief Loads the sound effect for an entity if not already loaded.
             * This method checks if the sound effect for an entity is loaded, and if not, it loads the sound
             * from the specified file path in the `SoundEffect` component.
             * @param i The index of the entity in the registry.
             * @param soundEffect Array of sound effect components for entities.
             */
            void loadSound(std::size_t i, SparseArray<component::SoundEffect> &soundEffect);
        };
    }
}

#endif /* !SOUNDSYSTEM_HPP_ */
