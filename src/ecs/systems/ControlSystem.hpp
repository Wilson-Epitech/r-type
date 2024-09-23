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

    #define NEGATIVE_VELOCITY -5
    #define POSITIVE_VELOCITY 5

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
        };
    }
}

#endif /* !CONTROLSYSTEM_HPP_ */
