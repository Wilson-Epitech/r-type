/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** DrawSystem
*/

/**
 * @file DrawSystem.hpp
 * @brief Declares the draw system responsible for rendering entities in the game engine ECS.
 * This file contains the `DrawSystem` class, which manages the rendering of entities, loading of textures,
 * and background drawing. It ensures that all drawable entities are properly displayed on the screen.
 */

#ifndef DRAWSYSTEM_HPP_
#define DRAWSYSTEM_HPP_

#include "Components.hpp"
#include "ISystem.hpp"

#define LETTER_SPACING 2

#define LIVES_BAR_HEIGHT 5

namespace ecs {
    namespace system {

        /**
         * @class DrawSystem
         * @brief System responsible for rendering entities in the game.
         * The `DrawSystem` handles the rendering of entities by drawing their textures on the screen.
         * It manages texture loading, background rendering, and ensures that entities are displayed correctly.
         */
        class DrawSystem : virtual public ecs::system::ISystem {
          public:
            DrawSystem() = default;
            ~DrawSystem() = default;

            /**
             * @brief Executes the draw system to render entities.
             * This method iterates over all entities and draws their corresponding textures on the screen.
             * It handles the positioning, scaling, and any necessary background rendering.
             * @param reg The ECS registry containing entities and their components.
             */
            void operator()(Registry &reg) override;

          private:
            /**
             * @brief Loads textures for entities if they are not yet loaded.
             * This method checks the drawable components of entities, and if the texture is not yet loaded,
             * it loads the texture from the specified path in the `SpriteInfos` component.
             * @param drawables Array of drawable components of entities.
             * @param spriteInfos Array of sprite information components of entities.
             */
            void loadTextures(SparseArray<component::Drawable> &drawables, SparseArray<component::SpriteInfos> &spriteInfos);

            /**
             * @brief Renders the background by drawing it multiple times if necessary.
             * This method handles the drawing of background entities, including adjusting the background
             * position and repeating the background texture to cover the screen if needed.
             * @param velocities Array of velocity components for entities.
             * @param drawables Array of drawable components for entities.
             * @param positions Array of position components for entities.
             * @param spriteInfos Array of sprite information components for entities.
             * @param index The index of the background entity being drawn.
             */
            void drawBackground(SparseArray<component::Velocity> &velocities, SparseArray<component::Drawable> &drawables,
                                SparseArray<component::Position> &positions, SparseArray<component::SpriteInfos> &spriteInfos,
                                const std::size_t &index);

            /**
             * @brief Updates the position of the background as it moves.
             * This method adjusts the position of the background texture based on the entity's velocity,
             * creating a smooth background movement effect.
             * @param velocities Array of velocity components for entities.
             * @param drawables Array of drawable components for entities.
             * @param positions Array of position components for entities.
             * @param spriteInfos Array of sprite information components for entities.
             * @param index The index of the background entity being updated.
             */
            void changeBackgroundPos(SparseArray<component::Velocity> &velocities, SparseArray<component::Drawable> &drawables,
                                     SparseArray<component::Position> &positions, SparseArray<component::SpriteInfos> &spriteInfos,
                                     const std::size_t &index);

            /**
             * @brief Adjusts the position of the new background segment.
             * This method calculates the new position for a background segment that is to be repeated
             * in the case where the background needs to fill the screen multiple times.
             * @param velocities Array of velocity components for entities.
             * @param drawables Array of drawable components for entities.
             * @param positions Array of position components for entities.
             * @param spriteInfos Array of sprite information components for entities.
             * @param index The index of the background entity.
             * @param j Index of the current segment being drawn.
             * @return Vector2 The new position for the background segment.
             */
            Vector2 changeNewBackgroundPos(SparseArray<component::Velocity> &velocities, SparseArray<component::Drawable> &drawables,
                                           SparseArray<component::Position> &positions, SparseArray<component::SpriteInfos> &spriteInfos,
                                           const std::size_t &index, int j);

            /**
             * @brief Calculates the number of times a background texture needs to be repeated to cover the screen.
             * @param spriteSize The size of the sprite being used as the background.
             * @param screenSize The size of the screen dimension (width or height).
             * @return int The number of repetitions required.
             */
            int getNbrRepeatBackground(float spriteSize, int screenSize);

            /**
             * @brief Renders the life of an entity.
             * @param reg The ECS Registry.
             * @param drawables Array of drawable components for entities.
             * @param spriteInfos Array of sprite information components for entities.
             * @param positions Array of position components for entities.
             * @param index The index of the entity life being drawn.
             */
            void drawLife(Registry &reg, SparseArray<component::Drawable> &drawables, SparseArray<component::SpriteInfos> &spriteInfos,
                          SparseArray<component::Position> &positions, std::size_t index);

            /**
            * @brief Renders the text of an entity.
            * @param reg The ECS Registry.
            */
            void drawText(Registry &reg);

            /**
             * @brief Load the font of a text.
             * @param textInfos The text informations of an entity.
             */
            void loadFonts(SparseArray<component::TextInfos> &textInfos);
        };
    }
}

#endif /* !DRAWSYSTEM_HPP_ */
