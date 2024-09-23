/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** ISystem
*/

#ifndef ISYSTEM_HPP_
    #define ISYSTEM_HPP_

    #include "Registry.hpp"

namespace ecs
{
    namespace system
    {
        class ISystem
        {
            public:
                ISystem() = default;
                ~ISystem() = default;

                virtual void operator()(Registry &reg) = 0;
        };
    }
}

#endif /* !ISYSTEM_HPP_ */
