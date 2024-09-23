/*
** R_TYPE PROJECT, 2024
** enemy
** File description:
** EnemySpawnSystem
*/

#ifndef ENEMYSPAWNSYSTEM_HPP_
    #define ENEMYSPAWNSYSTEM_HPP_

    #include "ISystem.hpp"
    #include "Components.hpp"

namespace ecs
{
    namespace system
    {
        class EnemySpawnSystem : virtual public ecs::system::ISystem
        {
            public:
                EnemySpawnSystem() = default;
                ~EnemySpawnSystem() = default;

                void operator()(Registry &reg) override;
            private:
                void createNewEnemy(component::EnemySpawn enemySpawn, component::Velocity enemyVelocities, component::Life enemyLives, Registry &reg);
        };
    }
}

#endif /* !ENEMYSPAWNSYSTEM_HPP_ */
