/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** CollisionSystem
*/

#ifndef COLLISIONSYSTEM_HPP_
    #define COLLISIONSYSTEM_HPP_

    #include "ISystem.hpp"
    #include "Components.hpp"
    #include <raylib.h>

namespace ecs
{
    namespace system
    {
        class CollisionSystem : virtual public ecs::system::ISystem
        {
            public:
                CollisionSystem() = default;
                ~CollisionSystem() = default;
                void operator()(Registry &reg) override
                {
                    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
                    SparseArray<component::Drawable> &drawables = reg.getComponents<component::Drawable>();
                    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
                    SparseArray<component::Lives> &lives = reg.getComponents<component::Lives>();

                    for (std::size_t i = 0; i < positions.size() && i < drawables.size() && i < types.size() && i < lives.size(); ++i) {
                        if (positions[i] && drawables[i] && types[i] && lives[i]) {
                            Rectangle rec1 = {positions[i]->x, positions[i]->y,
                                float(drawables[i]->spriteTexture.width), float(drawables[i]->spriteTexture.height)};
                            for (std::size_t j = 0; j < positions.size() && j < drawables.size() && j < types.size() && j < lives.size(); ++j) {
                                if (positions[j] && drawables[j] && types[j] && lives[j]) {
                                Rectangle rec2 = {positions[j]->x, positions[j]->y,
                                    float(drawables[j]->spriteTexture.width), float(drawables[j]->spriteTexture.height)};
                                    if (i != j && CheckCollisionRecs(rec1, rec2) && types[i]->type != types[j]->type
                                        && lives[i]->nbLives > 0 && lives[j]->nbLives > 0) {
                                            lives[i]->nbLives--;
                                            lives[j]->nbLives--;
                                    }
                                }
                            }
                        }
                    }
                }
        };
    }
}

#endif /* !COLLISIONSYSTEM_HPP_ */
