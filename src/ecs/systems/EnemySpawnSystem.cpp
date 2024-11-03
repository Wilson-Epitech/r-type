/*
** R_TYPE PROJECT, 2024
** enemy
** File description:
** EnemySpawnSystem
*/

#include "EnemySpawnSystem.hpp"

void ecs::system::EnemySpawnSystem::operator()(Registry &reg) {
    SparseArray<component::EnemySpawn> &enemySpawn = reg.getComponents<component::EnemySpawn>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    SparseArray<component::BossSpawn> &bossSpawns = reg.getComponents<component::BossSpawn>();

    std::chrono::steady_clock::time_point time0 = std::chrono::steady_clock::now();
    static std::vector<std::pair<std::size_t, std::chrono::steady_clock::time_point>> timer;
    static bool isFirstSpawn = true;
    bool isTimerSet = false;

    for (std::size_t i = 0; i < enemySpawn.size() && i < velocities.size() && i < lives.size() && i < spriteInfos.size(); ++i) {
        if (enemySpawn[i] && velocities[i] && lives[i] && spriteInfos[i]) {
            if (timer.size() > 0) {
                for (std::size_t j = 0; j < timer.size(); j++) {
                    if (timer[j].first == i) {
                        time0 = timer[j].second;
                        isTimerSet = true;
                    }
                }
            }
            if (!isTimerSet) {
                time0 = std::chrono::steady_clock::now();
                timer.push_back({i, time0});
            }
            std::size_t bossIndex = findLevel1BossEntity(reg);
            if (bossIndex != 0) {
                if (((Lib::isTimeElapsed(time0, enemySpawn[i]->timeBeforeSpawn) &&
                    (bossSpawns[bossIndex]->ongoingLevel != CurrentLevel::LEVEL1_BOSS &&
                    bossSpawns[findLevel1BossEntity(reg)]->ongoingLevel != CurrentLevel::LEVEL2_BOSS &&
                    bossSpawns[findLevel1BossEntity(reg)]->ongoingLevel != CurrentLevel::LEVEL3))) ||
                    isFirstSpawn) {
                    for (std::size_t j = 0; j < enemySpawn[i]->waveSize; j++) {
                        std::srand(rand() % time(NULL));
                        createNewEnemy(reg, enemySpawn, velocities, lives, spriteInfos, i);
                    }
                    isFirstSpawn = false;
                }
            }
            if (Lib::isTimeElapsed(time0, enemySpawn[i]->timeBeforeSpawn)) {
                for (std::size_t j = 0; j < timer.size(); j++) {
                    if (timer[j].first == i) {
                        timer[j].second = std::chrono::steady_clock::now();
                    }
                }
            }
        }
    }
}

void ecs::system::EnemySpawnSystem::createNewEnemy(Registry &reg, SparseArray<component::EnemySpawn> &enemySpawn,
                                                   SparseArray<component::Velocity> &velocities, SparseArray<component::Life> &lives,
                                                   SparseArray<component::SpriteInfos> &spriteInfos, const std::size_t &i) {
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    SparseArray<component::ShootingRate> &shootingRate = reg.getComponents<component::ShootingRate>();
    SparseArray<component::SoundEffectInfos> &soundEffectInfos = reg.getComponents<component::SoundEffectInfos>();
    SparseArray<component::MovementPattern> &movementPattern = reg.getComponents<component::MovementPattern>();
    entity_t entity = reg.spawnEntity();
    float x = (rand() % (enemySpawn[i]->maxSpawnWidth - enemySpawn[i]->minSpawnWidth)) + enemySpawn[i]->minSpawnWidth;
    float y = (rand() % (enemySpawn[i]->maxSpawnHeight - enemySpawn[i]->minSpawnHeight)) + enemySpawn[i]->minSpawnHeight;

    positions.insertAt(entity, ecs::component::Position{x, y});
    velocities.insertAt(entity, ecs::component::Velocity{velocities[i]->vx, velocities[i]->vy, velocities[i]->possibleVelocity});
    spriteInfos.insertAt(entity, ecs::component::SpriteInfos{spriteInfos[i]->spritePath, spriteInfos[i]->patternNb,
                                                             spriteInfos[i]->firstPattern, spriteInfos[i]->scaleFactor});
    types.insertAt(entity, ecs::component::Type{ecs::EntityType::ENEMY});
    lives.insertAt(entity, ecs::component::Life{lives[i]->nbLives, lives[i]->displayLife, lives[i]->hasToSummonPowerupAndWeapon});
    if (shootingRate.size() > i) {
        if (shootingRate[i])
            shootingRate.insertAt(entity, ecs::component::ShootingRate{shootingRate[i]->shootRate});
    }
    if (soundEffectInfos.size() > i) {
        if (soundEffectInfos[i])
            soundEffectInfos.insertAt(entity, ecs::component::SoundEffectInfos{soundEffectInfos[i]->soundPath});
    }
    if (movementPattern.size() > i) {
        if (movementPattern[i])
            movementPattern.insertAt(entity, ecs::component::MovementPattern{movementPattern[i].value()});
    }
}

std::size_t ecs::system::EnemySpawnSystem::checkCurrentScore(Registry &reg) {
    SparseArray<component::Score> &scores = reg.getComponents<component::Score>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();

    for (std::size_t i = 0; i < types.size(); i++) {
        if (types[i]->entityType == ecs::EntityType::TEXT && scores[i]->score != 0)
            return scores[i]->score;
    }
    return 0;
}

std::size_t ecs::system::EnemySpawnSystem::findLevel1BossEntity(Registry &reg) {
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();

    for (std::size_t i = 0; i < types.size(); i++) {
        if (types[i]->entityType == ecs::EntityType::BOSS)
            return i;
    }
    return 0;
}