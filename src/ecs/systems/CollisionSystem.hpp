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

                void operator()(Registry &reg) override;
            private:
                Rectangle spriteHitbox(SparseArray<component::Position> &positions,
                    SparseArray<component::Drawable> &drawables, std::size_t index);
                bool isPlayerAndPlayerShot(ecs::entityType &firstType, ecs::entityType &secondType);
        };
    }
}

#endif /* !COLLISIONSYSTEM_HPP_ */
