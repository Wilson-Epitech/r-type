/*
** R_TYPE PROJECT, 2024
** src
** File description:
** firingSystem
*/

#ifndef FIRINGSYSTEM_HPP_
    #define FIRINGSYSTEM_HPP_

    #include "ISystem.hpp"
    #include "Components.hpp"
    #include <iostream>

namespace ecs
{
    namespace system
    {
        class FiringSystem : virtual public ecs::system::ISystem
        {
            public:
                FiringSystem() = default;
                ~FiringSystem() = default;

                void operator()(Registry &reg) override
                {
                    SparseArray<component::Controllable> &controllables = reg.getComponents<component::Controllable>();
                    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();

                    for (std::size_t i = 0; i < controllables.size() && i < positions.size(); ++i) {
                        if (controllables[i] && positions[i]) {
                            if (!controllables[i]->isControllable)
                                continue;
                            if (IsKeyPressed(KEY_SPACE)) {
                                addShot(reg.spawnEntity(), positions[i]->x, positions[i]->y, reg);
                            }
                        }
                    }
                }
            private:
                void addShot(entity_t Entity, float x, float y, Registry &reg) {
                    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
                    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
                    SparseArray<component::Drawable> &drawables = reg.getComponents<component::Drawable>();

                    positions.insertAt(Entity, ecs::component::Position{x, y});
                    velocities.insertAt(Entity, ecs::component::Velocity{3.0f, 0});
                    drawables.insertAt(Entity, ecs::component::Drawable{"../assets/r-typesheet30a.gif"});
                }
        };
    }
}

#endif /* !FIRINGSYSTEM_HPP_ */
