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

namespace rtype
{
    class PositionSystem : virtual public rtype::ISystem
    {
        public:
            PositionSystem() = default;
            ~PositionSystem() = default;
            void operator()(registry &reg) override
            {
                sparse_array<component::Position> &positions = reg.get_components<component::Position>();
                sparse_array<component::Velocity> &velocities = reg.get_components<component::Velocity>();

                for (std::size_t i = 0; i < positions.size() && i < velocities.size(); ++i) {
                    if (positions[i] && velocities[i]) {
                        positions[i]->x += velocities[i]->vx;
                        positions[i]->y += velocities[i]->vy;
                    }
                }
            };
    };
}

#endif /* !POSITIONSYSTEM_HPP_ */
