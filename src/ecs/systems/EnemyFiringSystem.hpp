/*
** R_TYPE PROJECT, 2024
** branch
** File description:
** EnemyFiringSystem
*/

#ifndef ENEMYFIRINGSYSTEM_HPP_
    #define ENEMYFIRINGSYSTEM_HPP_

#include "Assets.hpp"
#include "Components.hpp"
#include "ISystem.hpp"

namespace ecs {
    namespace system {
        class EnemyFiringSystem : virtual public ecs::system::ISystem {
          public:
            EnemyFiringSystem() = default;
            ~EnemyFiringSystem() = default;

            void operator()(Registry &reg) override;

          private:
            void addShot(std::size_t index, Registry &reg);
            std::size_t getPlayerShootIndex(Registry &reg);
            void playSound(Registry &reg);
        };
    }
}

#endif /* !ENEMYFIRINGSYSTEM_HPP_ */
