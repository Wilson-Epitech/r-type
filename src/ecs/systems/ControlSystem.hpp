/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** ControlSystem
*/

#ifndef CONTROLSYSTEM_HPP_
    #define CONTROLSYSTEM_HPP_

    #include "ISystem.hpp"
    #include "Components.hpp"

    #define NEGATIVE_VELOCITY -0.2
    #define POSITIVE_VELOCITY 0.2

namespace rtype
{
    class ControlSystem : virtual public rtype::ISystem
    {
        public:
            ControlSystem() = default;
            ~ControlSystem() = default;
            void operator()(registry &reg) override
            {
                sparse_array<component::Controllable> &controllables = reg.get_components<component::Controllable>();
                sparse_array<component::Velocity> &velocities = reg.get_components<component::Velocity>();

                for (std::size_t i = 0; i < controllables.size() && i < velocities.size(); ++i) {
                    if (controllables[i] && velocities[i]) {
                        if (*controllables[i]->key == KeyPressed::UP) {
                            velocities[i]->vx = 0;
                            velocities[i]->vy = NEGATIVE_VELOCITY;
                        } else if (*controllables[i]->key == KeyPressed::DOWN) {
                            velocities[i]->vx = 0;
                            velocities[i]->vy = POSITIVE_VELOCITY;
                        } else if (*controllables[i]->key == KeyPressed::LEFT) {
                            velocities[i]->vx = NEGATIVE_VELOCITY;
                            velocities[i]->vy = 0;
                        } else if (*controllables[i]->key == KeyPressed::RIGHT) {
                            velocities[i]->vx = POSITIVE_VELOCITY;
                            velocities[i]->vy = 0;
                        }
                    }
                }
            };
    };
}

#endif /* !CONTROLSYSTEM_HPP_ */
