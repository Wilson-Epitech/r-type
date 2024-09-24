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
    double time0 = 0;
    static std::vector<std::pair<std::size_t, double>> timer;
    static bool isFirstSpawn = true;
    double time1 = GetTime();
    double timeElapsed = 0;
    float spawnInterval = 0;
    bool isTimerSet = false;

    for (std::size_t i = 0; i < enemySpawn.size() && i < enemyVelocities.size() && i < enemyLives.size() && i < enemySpriteInfos.size(); ++i) {
        if (enemySpawn[i] && enemyVelocities[i] && enemyLives[i] && enemySpriteInfos[i]) {
            if (timer.size() > 0) {
                for (std::size_t j = 0; j < timer.size(); j++) {
                    if (timer[j].first == i) {
                        time0 = timer[j].second;
                        isTimerSet = true;
                    }
                }
            }
            if (!isTimerSet) {
                time0 = GetTime();
                timer.push_back({i, time0});
            }
            timeElapsed = time1 - time0;
            spawnInterval = enemySpawn[i]->timeBeforeSpawn;
            if (timeElapsed >= spawnInterval || isFirstSpawn) {
                for (int j = 0; j < enemySpawn[i]->waveSize; j++) {
                    std::srand(rand() % time(NULL));
                    createNewEnemy(*enemySpawn[i], *enemyVelocities[i], *enemyLives[i], *enemySpriteInfos[i], reg);
                }
                isFirstSpawn = false;
            }
            if (timeElapsed >= spawnInterval) {
                for (std::size_t j = 0; j < timer.size(); j++) {
                    if (timer[j].first == i) {
                        timer[j].second = GetTime();
                    }
                }
            }
        }
    }
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
    float x = (rand() % (enemySpawn.maxSpawnWidth - enemySpawn.minSpawnWidth)) + enemySpawn.minSpawnWidth;
    float y = (rand() % (enemySpawn.maxSpawnHeight - enemySpawn.minSpawnHeight)) + enemySpawn.minSpawnHeight;

    positions.insertAt(entity, ecs::component::Position{x, y});
    velocities.insertAt(entity, ecs::component::Velocity(enemyVelocities));
    spriteInfos.insertAt(entity, ecs::component::SpriteInfos(enemySpriteInfos));
    drawables.insertAt(entity, ecs::component::Drawable{});
    types.insertAt(entity, ecs::component::Type{ecs::EntityType::ENEMY});
    lives.insertAt(entity, ecs::component::Life(enemyLives));
}
