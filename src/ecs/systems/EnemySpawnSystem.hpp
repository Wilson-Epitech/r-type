/*
** R_TYPE PROJECT, 2024
** enemy
** File description:
** EnemySpawnSystem
*/

#ifndef ENEMYSPAWNSYSTEM_HPP_
#define ENEMYSPAWNSYSTEM_HPP_

#include "Components.hpp"
#include "ISystem.hpp"

namespace ecs {
    namespace system {
        class EnemySpawnSystem : virtual public ecs::system::ISystem {
          public:
            EnemySpawnSystem() = default;
            ~EnemySpawnSystem() = default;

            void operator()(Registry &reg) override;

          private:
            void createNewEnemy(Registry &reg, SparseArray<component::EnemySpawn> &enemySpawn,
                                                               SparseArray<component::Velocity> &velocities,
                                                               SparseArray<component::Life> &lives,
                                                               SparseArray<component::SpriteInfos> &spriteInfos, const std::size_t &i);
        };
    }
}

#endif /* !ENEMYSPAWNSYSTEM_HPP_ */
