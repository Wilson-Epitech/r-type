/*
** R_TYPE PROJECT, 2024
** enemy_wave
** File description:
** SoundSystem
*/

#ifndef SOUNDSYSTEM_HPP_
#define SOUNDSYSTEM_HPP_

#include "Components.hpp"
#include "ISystem.hpp"

#define SPRITE_UPDATE_TIME 0.1

namespace ecs {
    namespace system {
        class SoundSystem : virtual public ecs::system::ISystem {
          public:
            SoundSystem() = default;
            ~SoundSystem() = default;

            void operator()(Registry &reg) override;
        };
    }
}

#endif /* !SOUNDSYSTEM_HPP_ */
