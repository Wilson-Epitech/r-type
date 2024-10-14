/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** DrawSystem
*/

#ifndef DRAWSYSTEM_HPP_
#define DRAWSYSTEM_HPP_

#include "Components.hpp"
#include "ISystem.hpp"

#define LIVES_BAR_HEIGHT 5

namespace ecs {
    namespace system {
        class DrawSystem : virtual public ecs::system::ISystem {
          public:
            DrawSystem() = default;
            ~DrawSystem() = default;

            void operator()(Registry &reg) override;

          private:
            void loadTextures(SparseArray<component::Drawable> &drawables, SparseArray<component::SpriteInfos> &spriteInfos);
            void drawBackground(SparseArray<component::Velocity> &velocities, SparseArray<component::Drawable> &drawables,
                                SparseArray<component::Position> &positions, SparseArray<component::SpriteInfos> &spriteInfos,
                                const std::size_t &index);
            void changeBackgroundPos(SparseArray<component::Velocity> &velocities, SparseArray<component::Drawable> &drawables,
                                     SparseArray<component::Position> &positions, SparseArray<component::SpriteInfos> &spriteInfos,
                                     const std::size_t &index);
            Vector2 changeNewBackgroundPos(SparseArray<component::Velocity> &velocities, SparseArray<component::Drawable> &drawables,
                                           SparseArray<component::Position> &positions, SparseArray<component::SpriteInfos> &spriteInfos,
                                           const std::size_t &index, int j);
            int getNbrRepeatBackground(float spriteSize, int screenSize);
            void drawLife(Registry &reg, SparseArray<component::Drawable> &drawables, SparseArray<component::SpriteInfos> &spriteInfos,
                          SparseArray<component::Position> &positions, std::size_t index);
        };
    }
}

#endif /* !DRAWSYSTEM_HPP_ */
