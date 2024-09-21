/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** PositionSystem
*/

#ifndef POSITIONSYSTEM_HPP_
    #define POSITIONSYSTEM_HPP_

    #include "ISystem.hpp"
    #include "Components.hpp"

namespace ecs
{
    namespace system
    {
        class PositionSystem : virtual public ecs::system::ISystem
        {
            public:
                PositionSystem() = default;
                ~PositionSystem() = default;
                void operator()(Registry &reg) override
                {
                    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
                    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();

                    for (std::size_t i = 0; i < positions.size() && i < velocities.size(); ++i) {
                        if (positions[i] && velocities[i]) {
                            positions[i]->x += velocities[i]->vx;
                            positions[i]->y += velocities[i]->vy;
                        }
                    }
                }
        };
    }
}

#endif /* !POSITIONSYSTEM_HPP_ */
