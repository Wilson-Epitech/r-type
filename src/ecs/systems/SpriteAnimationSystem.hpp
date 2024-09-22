/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** DrawSystem
*/

#ifndef SPRITEANIMATIONSYSTEM_HPP_
    #define SPRITEANIMATIONSYSTEM_HPP_

    #include "ISystem.hpp"
    #include "Components.hpp"

namespace ecs
{
    namespace system
    {
        class SpriteAnimationSystem : virtual public ecs::system::ISystem
        {
            public:
                SpriteAnimationSystem() = default;
                ~SpriteAnimationSystem() = default;

                void operator()(Registry &reg) override;
        };
    }
}

#endif /* !SPRITEANIMATIONSYSTEM_HPP_ */
