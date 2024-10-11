/*
** R_TYPE PROJECT, 2024
** branch
** File description:
** MusicSystem
*/

#ifndef MUSICSYSTEM_HPP_
#define MUSICSYSTEM_HPP_

#include "Components.hpp"
#include "ISystem.hpp"

namespace ecs {
    namespace system {
        class MusicSystem : virtual public ecs::system::ISystem {
          public:
            MusicSystem() = default;
            ~MusicSystem() = default;

            void operator()(Registry &reg) override;

          private:
            void loadMusic(std::size_t i, SparseArray<component::BackgroundMusic> &music);
        };
    }
}

#endif /* !MUSICSYSTEM_HPP_ */
