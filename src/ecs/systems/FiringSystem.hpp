/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** FiringSystem
*/

#ifndef FIRINGSYSTEM_HPP_
    #define FIRINGSYSTEM_HPP_

    #include "ISystem.hpp"
    #include "Components.hpp"
    #include "Assets.hpp"
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

                void operator()(Registry &reg) override;
            private:
                void addShot(entity_t entity, float x, float y, Registry &reg);
        };
    }
}

#endif /* !FIRINGSYSTEM_HPP_ */
