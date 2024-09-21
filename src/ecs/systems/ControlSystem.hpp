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

namespace ecs {
    class ControlSystem : virtual public ecs::ISystem {
        public:
            ControlSystem() = default;
            ~ControlSystem() = default;
            void operator()(Registry &reg) override
            {
                SparseArray<component::Controllable> &controllables = reg.getComponents<component::Controllable>();
                SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();

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
