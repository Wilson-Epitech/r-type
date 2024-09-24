/*
** R_TYPE PROJECT, 2024
** enemy
** File description:
** EnemySpawnSystem
*/

#include "EnemySpawnSystem.hpp"

void ecs::system::EnemySpawnSystem::operator()(Registry &reg)
{
    SparseArray<component::EnemySpawn> &enemySpawn = reg.getComponents<component::EnemySpawn>();
    SparseArray<component::Velocity> &enemyVelocities = reg.getComponents<component::Velocity>();
    SparseArray<component::Life> &enemyLives = reg.getComponents<component::Life>();
    SparseArray<component::SpriteInfos> &enemySpriteInfos = reg.getComponents<component::SpriteInfos>();
    static double time0 = GetTime();
    double time1 = GetTime();
    double timeElapsed = time1 - time0;
    float spawnInterval = 0;

    for (std::size_t i = 0; i < enemySpawn.size() && i < enemyVelocities.size() && i < enemyLives.size() && i < enemySpriteInfos.size(); ++i) {
        if (enemySpawn[i] && enemyVelocities[i] && enemyLives[i] && enemySpriteInfos[i]) {
            spawnInterval = enemySpawn[i]->timeBeforeSpawn;
            if (timeElapsed >= spawnInterval) {
                createNewEnemy(*enemySpawn[i], *enemyVelocities[i], *enemyLives[i], *enemySpriteInfos[i], reg);
            }
        }
    }
    if (timeElapsed >= spawnInterval)
        time0 = GetTime();
}

void ecs::system::EnemySpawnSystem::createNewEnemy(component::EnemySpawn enemySpawn, component::Velocity enemyVelocities, component::Life enemyLives, component::SpriteInfos enemySpriteInfos, Registry &reg)
{
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    SparseArray<component::Drawable> &drawables = reg.getComponents<component::Drawable>();
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();
    entity_t entity = reg.spawnEntity();
    float x = 0;
    float y = 0;

    std::srand(time(NULL));
    x = (rand() % (enemySpawn.maxSpawnWidth - enemySpawn.minSpawnWidth)) + enemySpawn.minSpawnWidth;
    y = (rand() % (enemySpawn.maxSpawnHeight - enemySpawn.minSpawnHeight)) + enemySpawn.minSpawnHeight;
    positions.insertAt(entity, ecs::component::Position{x, y});
    velocities.insertAt(entity, ecs::component::Velocity(enemyVelocities));
    spriteInfos.insertAt(entity, ecs::component::SpriteInfos(enemySpriteInfos));
    drawables.insertAt(entity, ecs::component::Drawable{});
    types.insertAt(entity, ecs::component::Type{ecs::EntityType::ENEMY});
    lives.insertAt(entity, ecs::component::Life(enemyLives));
}
