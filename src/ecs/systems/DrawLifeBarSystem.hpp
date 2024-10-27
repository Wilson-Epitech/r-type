/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** DrawLifeBarSystem
*/

#ifndef DRAWLIFEBARSYSTEM_HPP_
#define DRAWLIFEBARSYSTEM_HPP_

#include "Components.hpp"
#include "RaylibComponents.hpp"
#include "ISystem.hpp"

#define LIVES_BAR_HEIGHT 5 ///< The height of lives bar displayed

namespace ecs {
    namespace system {

        /**
         * @class DrawLifeBarSystem
         * @brief System responsible for rendering entities lives bar in the game.
         * The `DrawLifeBarSystem` handles the rendering of entities lives bar by drawing them on the screen.
         */
        class DrawLifeBarSystem : virtual public ecs::system::ISystem {
          public:
            DrawLifeBarSystem() = default;
            ~DrawLifeBarSystem() = default;

            /**
             * @brief Executes the draw life system to render entities lives bar.
             * This method iterates over all entities and draws their corresponding lives bar on the screen if it should be.
             * @param reg The ECS registry containing entities and their components.
             */
            void operator()(Registry &reg) override;

          private:
            /**
             * @brief Renders the life bar of an entity.
             * @param drawables Array of drawable components for entities.
             * @param spriteInfos Array of sprite information components for entities.
             * @param positions Array of position components for entities.
             * @param lives Array of lives components for entities.
             * @param index The index of the entity life being drawn.
             */
            void drawLife(SparseArray<component::Drawable> &drawables, SparseArray<component::SpriteInfos> &spriteInfos,
                          SparseArray<component::Position> &positions, SparseArray<component::Life> &lives, const std::size_t &index);
        };
    }
}

#endif /* !DRAWLIFEBARSYSTEM_HPP_ */
