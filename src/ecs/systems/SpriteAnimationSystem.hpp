/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** SpriteAnimationSystem
*/

/**
 * @file SpriteAnimationSystem.hpp
 * @brief Declares the system responsible for handling sprite animations in the game engine ECS.
 * This file contains the `SpriteAnimationSystem` class, which manages the updating of sprite animations
 * based on time. It ensures that sprite frames are updated at a regular interval to create animated visuals.
 */

#ifndef SPRITEANIMATIONSYSTEM_HPP_
#define SPRITEANIMATIONSYSTEM_HPP_

#include "Components.hpp"
#include "ISystem.hpp"

#define SPRITE_UPDATE_TIME 0.1 ///< Time interval (in seconds) between sprite frame updates.

namespace ecs {
    namespace system {

        /**
         * @class SpriteAnimationSystem
         * @brief System responsible for updating sprite animations.
         * The `SpriteAnimationSystem` handles the timing and updating of sprite animations in the game.
         * It ensures that sprites are animated by changing their frames at regular intervals, providing smooth
         * visual transitions between sprite frames.
         */
        class SpriteAnimationSystem : virtual public ecs::system::ISystem {
          public:
            SpriteAnimationSystem() = default;
            ~SpriteAnimationSystem() = default;

            /**
             * @brief Executes the sprite animation system to update sprite frames.
             * This function updates the sprite frames for entities that have animations. It ensures that
             * the animation progresses based on the defined update time, providing the visual effect of animation.
             * @param reg The ECS registry containing entities and their components.
             */
            void operator()(Registry &reg) override;
        };
    }
}

#endif /* !SPRITEANIMATIONSYSTEM_HPP_ */
