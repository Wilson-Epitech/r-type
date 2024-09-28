/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** CollisionSystem
*/

#ifndef COLLISIONSYSTEM_HPP_
#define COLLISIONSYSTEM_HPP_

#include "Components.hpp"
#include "ISystem.hpp"
#include <raylib.h>

namespace ecs {
    namespace system {
        class CollisionSystem : virtual public ecs::system::ISystem {
          public:
            CollisionSystem() = default;
            ~CollisionSystem() = default;

            void operator()(Registry &reg) override;

          private:
            Rectangle spriteHitbox(SparseArray<component::Position> &positions, SparseArray<component::Drawable> &drawables,
                                   std::size_t index);
            bool isPlayerAndPlayerShot(ecs::EntityType &firstType, ecs::EntityType &secondType);
            bool isEnemyAndEnemyShot(ecs::EntityType &firstType, ecs::EntityType &secondType);
            bool isShotAndShot(ecs::EntityType &firstType, ecs::EntityType &secondType);
        };
    }
}

#endif /* !COLLISIONSYSTEM_HPP_ */
