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

    #define SCREEN_WIDTH 1920
    #define SCREEN_HEIGHT 1080

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
                void loadTextures(SparseArray<component::Drawable> &drawables);
                void drawBackground(component::Velocity &velocity, component::Drawable &drawables, component::Position &positions);
                void changeBackgroundPos(component::Velocity &velocity, component::Drawable &drawables, component::Position &positions);
                Vector2 changeNewBackgroundPos(component::Velocity &velocity, component::Drawable &drawables, component::Position &positions, int i);
                int getNbrRepeatBackgroundX(component::Drawable &drawables);
                int getNbrRepeatBackgroundY(component::Drawable &drawables);
        };
    }
}

#endif /* !DRAWSYSTEM_HPP_ */
