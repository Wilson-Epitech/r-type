/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** DrawSystem
*/

#ifndef DRAWSYSTEM_HPP_
    #define DRAWSYSTEM_HPP_

    #include "ISystem.hpp"
    #include "Components.hpp"

namespace ecs
{
    namespace system
    {
        class DrawSystem : virtual public ecs::system::ISystem
        {
            public:
                DrawSystem() = default;
                ~DrawSystem() = default;

                void operator()(Registry &reg) override;
            private:
                void loadTextures(SparseArray<component::Drawable> &drawables, SparseArray<component::SpriteInfos> &spriteInfos);
                void drawBackground(component::Velocity &velocities, component::Drawable &drawables, component::Position &positions, component::SpriteInfos &spriteInfos);
                void changeBackgroundPos(component::Velocity &velocities, component::Drawable &drawables, component::Position &positions, component::SpriteInfos &spriteInfos);
                Vector2 changeNewBackgroundPos(component::Velocity &velocities, component::Drawable &drawables, component::Position &positions, component::SpriteInfos &spriteInfos, int i);
                int getNbrRepeatBackground(float spriteSize, int screenSize);
        };
    }
}

#endif /* !DRAWSYSTEM_HPP_ */
