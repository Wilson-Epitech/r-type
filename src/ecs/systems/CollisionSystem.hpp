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
                    Rectangle rec1 = {0};
                    Rectangle rec2 = {0};

                    for (std::size_t i = 0; i < positions.size() && i < drawables.size() && i < types.size() && i < lives.size(); ++i) {
                        if (!positions[i] || !drawables[i] || !types[i] || !lives[i])
                            continue;
                        if (lives[i]->nbLives == 0)
                            continue;
                        rec1 = spriteHitbox(positions, drawables, i);
                        for (std::size_t j = 0; j < positions.size() && j < drawables.size() && j < types.size() && j < lives.size(); ++j) {
                            if (!positions[j] || !drawables[j] || !types[j] || !lives[j])
                                continue;
                            if (i == j || types[i]->type == types[j]->type || lives[j]->nbLives == 0)
                                continue;
                            rec2 = spriteHitbox(positions, drawables, j);
                            if (CheckCollisionRecs(rec1, rec2) && !isPlayerAndPlayerShot(types[i]->type, types[j]->type)) {
                                lives[i]->nbLives--;
                                lives[j]->nbLives--;
                            }
                        }
                    }
                }
            private:
                Rectangle spriteHitbox(SparseArray<component::Position> &positions,
                    SparseArray<component::Drawable> &drawables, std::size_t index) {
                    return Rectangle {positions[index]->x, positions[index]->y,
                       float(drawables[index]->spriteTexture.width), float(drawables[index]->spriteTexture.height)};
                    }

                bool isPlayerAndPlayerShot(ecs::entityType &firstType, ecs::entityType &secondType) {
                    return (firstType == ecs::entityType::PLAYER && secondType == ecs::entityType::PLAYERSHOT ||
                        firstType == ecs::entityType::PLAYERSHOT && secondType == ecs::entityType::PLAYER);
                }
        };
    }
}

#endif /* !COLLISIONSYSTEM_HPP_ */
