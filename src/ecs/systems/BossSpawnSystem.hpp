/*
** R_TYPE PROJECT, 2024
** enemy
** File description:
** EnemySpawnSystem
*/

/**
 * @file EnemySpawnSystem.hpp
 * @brief Declares the system responsible for spawning enemy entities in the game engine ECS.
 * This file contains the `EnemySpawnSystem` class, which handles the logic of spawning enemies in waves
 * at specific intervals or under certain conditions.
 */

#ifndef BOSSSPAWNSYSTEM_HPP_
#define BOSSSPAWNSYSTEM_HPP_

#include "Components.hpp"
#include "ISystem.hpp"

namespace ecs {
    namespace system {

        class BossSpawnSystem : virtual public ecs::system::ISystem {
          public:
            BossSpawnSystem() = default;
            ~BossSpawnSystem() = default;

            void operator()(Registry &reg) override;

          private:

            void createNewEnemy(Registry &reg, SparseArray<component::Type> &types, const std::size_t &i);
            int findLevel1BossEntity(SparseArray<component::Type> &types);
        };
    }
}

#endif /* !BOSSSPAWNSYSTEM_HPP_ */
