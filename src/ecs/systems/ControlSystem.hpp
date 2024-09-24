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

    #define JOYSTICK_SENSIBILITY 0.6

namespace ecs
{
    namespace system
    {
        class ControlSystem : virtual public ecs::system::ISystem
        {
            public:
                ControlSystem() = default;
                ~ControlSystem() = default;

                void operator()(Registry &reg) override;
            private:
                void handleController(int &gamepad, SparseArray<component::Velocity> &velocities, const std::size_t &i);
        };
    }
}

#endif /* !CONTROLSYSTEM_HPP_ */
