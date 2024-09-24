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

    #define SHOOT_RATE 0.3

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
                void addShot(std::size_t index, Registry &reg);
                std::pair<component::SpriteInfos,component::Velocity> getPlayerShoot(Registry &reg);
                bool IsGamepadTriggerButtonDown(int &gamepad);
        };
    }
}

#endif /* !FIRINGSYSTEM_HPP_ */
