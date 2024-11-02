/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** BossSpawnSystem
*/

#include "BossSpawnSystem.hpp"

#include <iostream>

void ecs::system::BossSpawnSystem::operator()(Registry &reg) {
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    SparseArray<component::Score> &scores = reg.getComponents<component::Score>();
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    SparseArray<component::BossSpawn> &bossSpawns = reg.getComponents<component::BossSpawn>();
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    static std::chrono::steady_clock::time_point time0 = std::chrono::steady_clock::now();
    static bool isTimerSet = false;
    static bool bossSpawned = false;
    static std::size_t finalLevelBoss = 0;
    std::size_t bossEntity = findChosenBossEntity(types, ecs::EntityType::BOSS);

    for (std::size_t i = 0; i < types.size() && i < scores.size() && i < positions.size() && i < velocities.size(); ++i) {
        if (!types[i])
            continue;
        if (types[i]->entityType == ecs::EntityType::TEXT && (scores[i]->score == SCORE_FINAL_BOSS_LEVEL1 || scores[i]->score == SCORE_FINAL_BOSS_LEVEL2) && !isTimerSet) {
            if (!isTimerSet) {
                bossSpawned = false;
                time0 = std::chrono::steady_clock::now();
                isTimerSet = true;
                if (bossEntity < bossSpawns.size()) {
                    if (bossSpawns[bossEntity]) {
                        if (bossSpawns[bossEntity]->ongoingLevel == CurrentLevel::LEVEL1)
                            bossSpawns[bossEntity]->ongoingLevel = CurrentLevel::LEVEL1_BOSS;
                        if (bossSpawns[bossEntity]->ongoingLevel == CurrentLevel::LEVEL2)
                            bossSpawns[bossEntity]->ongoingLevel = CurrentLevel::LEVEL2_BOSS;
                    }
                }
            }
        }
        if (isTimerSet && !bossSpawned) {
            if (Lib::isTimeElapsed(time0, TIME_WAIT_LEVEL1_BOSS)) {
                std::size_t bossEntity = findChosenBossEntity(types, ecs::EntityType::BOSS);
                if (bossEntity != 0) {
                    if (bossSpawns[bossEntity]->ongoingLevel == CurrentLevel::LEVEL1_BOSS ||
                        bossSpawns[bossEntity]->ongoingLevel == CurrentLevel::LEVEL2_BOSS) {
                        if (bossSpawns[bossEntity]->ongoingLevel == CurrentLevel::LEVEL2_BOSS) {
                            spriteInfos[bossEntity]->spritePath = LEVEL2_BOSS_PATH;
                            spriteInfos[bossEntity]->isLoaded = false;
                            spriteInfos[bossEntity]->patternNb = LEVEL2_BOSS_NB_PATTERN;
                            spriteInfos[bossEntity]->scaleFactor = LEVEL2_BOSS_SCALE_FACTOR;
                        }
                        createNewBoss(reg, types, bossEntity, LEVEL1_BOSS_SPAWN_X, LEVEL1_BOSS_SPAWN_Y, finalLevelBoss);
                        bossSpawned = true;
                        isTimerSet = false;
                    }
                }
            }
        }
        if (bossSpawns[bossEntity]->ongoingLevel == CurrentLevel::LEVEL3 && finalLevelBoss < 4) {
            if (finalLevelBoss == 0) {
                positions[bossEntity]->x = 1200;
                positions[bossEntity]->y = 200;
            } else {
                positions[bossEntity]->y += 200;
            }
            spriteInfos[bossEntity]->spritePath = "assets/bossLevel3.png";
            spriteInfos[bossEntity]->isLoaded = false;
            spriteInfos[bossEntity]->patternNb = 1;
            spriteInfos[bossEntity]->scaleFactor = 1.2;
            createNewBoss(reg, types, bossEntity, positions[bossEntity]->x, positions[bossEntity]->y, finalLevelBoss);
            finalLevelBoss += 1;
        }
        if (!positions[i])
            continue;
        if (bossSpawned && types[i]->entityType == ecs::EntityType::BOSS && positions[i]->x <= LEVEL1_BOSS_STOP) {
            if (velocities[i])
                velocities[i]->vx = 0;
        }
    }
}

void ecs::system::BossSpawnSystem::createNewBoss(Registry &reg, SparseArray<component::Type> &types, const std::size_t &i, const float &x, const float &y, const std::size_t &level3BossIndex) {
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::ShootingRate> &shootingRate = reg.getComponents<component::ShootingRate>();
    SparseArray<component::SoundEffectInfos> &soundEffect = reg.getComponents<component::SoundEffectInfos>();
    SparseArray<component::MovementPattern> &movementPatterns = reg.getComponents<component::MovementPattern>();
    SparseArray<component::BossSpawn> &bossSpawns = reg.getComponents<component::BossSpawn>();

    entity_t entity = reg.spawnEntity();

    positions.insertAt(entity, ecs::component::Position{x, y});
    velocities.insertAt(entity, ecs::component::Velocity{LEVEL1_BOSS_VELOCITY, 0, 0});
    spriteInfos.insertAt(entity, ecs::component::SpriteInfos{spriteInfos[i]->spritePath, spriteInfos[i]->patternNb,
                                                             spriteInfos[i]->firstPattern, spriteInfos[i]->scaleFactor});
    types.insertAt(entity, ecs::component::Type{ecs::EntityType::BOSS});
    lives.insertAt(entity, ecs::component::Life{lives[i]->nbLives, lives[i]->displayLife, lives[i]->hasToSummonPowerup});
    if (shootingRate.size() > i) {
        if (shootingRate[i])
            shootingRate.insertAt(entity, ecs::component::ShootingRate{shootingRate[i]->shootRate});
    }
    if (soundEffect.size() > i) {
        if (soundEffect[i])
            soundEffect.insertAt(entity, ecs::component::SoundEffectInfos{soundEffect[i]->soundPath});
    }
    movementPatterns[i]->movementPatternType = ecs::MovementType::STRAIGHT;
}

std::size_t ecs::system::BossSpawnSystem::findChosenBossEntity(SparseArray<component::Type> &types, const ecs::EntityType &entityToFind)
{
    for (std::size_t i = 0; i < types.size(); i++) {
        if (types[i]->entityType == entityToFind)
            return i;
    }
    return 0;
}
