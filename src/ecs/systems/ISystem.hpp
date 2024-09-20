/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** ISystem
*/

#ifndef ISYSTEM_HPP_
    #define ISYSTEM_HPP_

    #include "Registry.hpp"

namespace rtype
{
    class ISystem
    {
        public:
            ISystem() = default;
            ~ISystem() = default;
            virtual void operator()(registry &reg) = 0;
    };
}

#endif /* !ISYSTEM_HPP_ */
