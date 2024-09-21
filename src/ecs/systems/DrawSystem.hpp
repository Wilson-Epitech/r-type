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
                void operator()(Registry &reg) override
                {
                    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
                    SparseArray<component::Drawable> &drawables = reg.getComponents<component::Drawable>();

                    loadTextures(drawables);
                    for (std::size_t i = 0; i < drawables.size() && i < positions.size(); ++i) {
                        if (drawables[i] && positions[i]) {
                            if (drawables[i]->spritePath.length() > 0) {
                                DrawTexture(drawables[i]->spriteTexture, positions[i]->x, positions[i]->y, RAYWHITE);
                            }
                        }
                    }
                }

                void loadTextures(SparseArray<component::Drawable> &drawables)
                {
                    static bool isLoaded = false;

                    if (isLoaded)
                        return;
                    for (std::size_t i = 0; i < drawables.size(); ++i) {
                        if (drawables[i]) {
                            if (drawables[i]->spritePath.length() > 0) {
                                drawables[i]->spriteTexture = LoadTexture(drawables[i]->spritePath.c_str());
                            }
                        }
                    }
                    isLoaded = true;
                }
        };
    }
}

#endif /* !DRAWSYSTEM_HPP_ */
