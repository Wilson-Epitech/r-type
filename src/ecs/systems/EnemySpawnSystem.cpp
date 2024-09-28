/*
** R_TYPE PROJECT, 2024
** enemy
** File description:
** EnemySpawnSystem
*/

#include "EnemySpawnSystem.hpp"

void ecs::system::EnemySpawnSystem::operator()(Registry &reg) {
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

    for (std::size_t i = 0; i < enemySpawn.size() && i < enemyVelocities.size() && i < enemyLives.size() && i < enemySpriteInfos.size();
         ++i) {
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
                    createNewEnemy(reg, enemySpawn, enemyVelocities, enemyLives, enemySpriteInfos, i);
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

void ecs::system::EnemySpawnSystem::createNewEnemy(Registry &reg, SparseArray<component::EnemySpawn> enemySpawn,
                                                   SparseArray<component::Velocity> enemyVelocities,
                                                   SparseArray<component::Life> enemyLives,
                                                   SparseArray<component::SpriteInfos> enemySpriteInfos, const std::size_t &i) {
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    SparseArray<component::Drawable> &drawables = reg.getComponents<component::Drawable>();
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();
    SparseArray<component::ShootingRate> &enemyShooting = reg.getComponents<component::ShootingRate>();
    SparseArray<component::SoundEffect> &enemySoundEffect = reg.getComponents<component::SoundEffect>();
    entity_t entity = reg.spawnEntity();
    float x = (rand() % (enemySpawn[i]->maxSpawnWidth - enemySpawn[i]->minSpawnWidth)) + enemySpawn[i]->minSpawnWidth;
    float y = (rand() % (enemySpawn[i]->maxSpawnHeight - enemySpawn[i]->minSpawnHeight)) + enemySpawn[i]->minSpawnHeight;

    positions.insertAt(entity, ecs::component::Position{x, y});
    velocities.insertAt(entity,
                        ecs::component::Velocity{enemyVelocities[i]->vx, enemyVelocities[i]->vy, enemyVelocities[i]->possibleVelocity});
    spriteInfos.insertAt(entity, ecs::component::SpriteInfos{enemySpriteInfos[i]->spritePath, enemySpriteInfos[i]->patternNb,
                                                             enemySpriteInfos[i]->firstPattern, enemySpriteInfos[i]->scaleFactor});
    drawables.insertAt(entity, ecs::component::Drawable{});
    types.insertAt(entity, ecs::component::Type{ecs::EntityType::ENEMY});
    lives.insertAt(entity, ecs::component::Life{enemyLives[i]->nbLives});
    if (enemyShooting.size() > i) {
        if (enemyShooting[i])
            enemyShooting.insertAt(entity, ecs::component::ShootingRate{enemyShooting[i]->shootRate});
    }
    if (enemySoundEffect[i])
        enemySoundEffect.insertAt(entity, ecs::component::SoundEffect{enemySoundEffect[i]->soundPath});
}
