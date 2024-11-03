/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** BossSpawnSystem
*/

#include "BossSpawnSystem.hpp"

void ecs::system::BossSpawnSystem::operator()(Registry &reg) {
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    SparseArray<component::Score> &scores = reg.getComponents<component::Score>();
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    SparseArray<component::BossSpawn> &bossSpawns = reg.getComponents<component::BossSpawn>();
    static std::chrono::steady_clock::time_point time0 = std::chrono::steady_clock::now();
    static bool isTimerSet = false;
    static bool bossSpawned = false;
    std::size_t bossEntity = findChosenEntity(types, ecs::EntityType::BOSS);

    for (std::size_t i = 0; i < types.size() && i < scores.size() && i < positions.size() && i < velocities.size(); ++i) {
        if (!types[i])
            continue;
        if (types[i]->entityType == ecs::EntityType::TEXT && (scores[i]->score == SCORE_FINAL_BOSS_LEVEL1 || (scores[i]->score >= SCORE_FINAL_BOSS_LEVEL2 && !bossSpawned)) && !isTimerSet) {
            if (!isTimerSet) {
                bossSpawned = false;
                time0 = std::chrono::steady_clock::now();
                isTimerSet = true;
                updateOngoingLevelStatus(reg, types);
            }
        }
        if (isTimerSet && !bossSpawned) {
            if (Lib::isTimeElapsed(time0, TIME_WAIT_LEVEL1_BOSS)) {
                if (bossEntity != 0) {
                    if (bossSpawns[bossEntity]->ongoingLevel == CurrentLevel::LEVEL1_BOSS ||
                        bossSpawns[bossEntity]->ongoingLevel == CurrentLevel::LEVEL2_BOSS) {
                        changeSpriteLevel2Boss(reg, types);
                        createNewBoss(reg, types, bossEntity);
                        bossSpawned = true;
                        isTimerSet = false;
                    }
                }
            }
        }
        spawnBossLevel3(reg, types, bossEntity);
        if (!positions[i])
            continue;
        if (bossSpawned && types[i]->entityType == ecs::EntityType::BOSS && positions[i]->x <= LEVEL1_BOSS_STOP) {
            if (velocities[i])
                velocities[i]->vx = 0;
        }
    }
}

void ecs::system::BossSpawnSystem::spawnBossLevel3(Registry &reg, SparseArray<component::Type> &types, const std::size_t &bossEntity)
{
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    SparseArray<component::BossSpawn> &bossSpawns = reg.getComponents<component::BossSpawn>();
    std::size_t bossLevel2Index = findChosenEntity(types, ecs::EntityType::BOSS_LEVEL3_TEMPLATE);
    static bool finalLevelBoss = false;

    if (bossEntity < bossSpawns.size() && bossEntity < spriteInfos.size()) {
        if (!bossSpawns[bossEntity] || !spriteInfos[bossEntity])
            return;
        if (bossSpawns[bossEntity]->ongoingLevel == CurrentLevel::LEVEL3 && !finalLevelBoss) {
            spriteInfos[bossEntity]->spritePath = spriteInfos[bossLevel2Index]->spritePath;
            spriteInfos[bossEntity]->isLoaded = spriteInfos[bossLevel2Index]->isLoaded;
            spriteInfos[bossEntity]->patternNb = spriteInfos[bossLevel2Index]->patternNb;
            spriteInfos[bossEntity]->scaleFactor = spriteInfos[bossLevel2Index]->scaleFactor;
            createNewBoss(reg, types, bossEntity);
            finalLevelBoss = true;
            bossSpawns[bossEntity]->hasBeenSlayed = bossSpawns[bossLevel2Index]->hasBeenSlayed;
        }
    }
}

void ecs::system::BossSpawnSystem::updateOngoingLevelStatus(Registry &reg, SparseArray<component::Type> &types)
{
    SparseArray<component::BossSpawn> &bossSpawns = reg.getComponents<component::BossSpawn>();
    std::size_t bossEntity = findChosenEntity(types, ecs::EntityType::BOSS);

    if (bossEntity < bossSpawns.size()) {
        if (bossSpawns[bossEntity]) {
            if (bossSpawns[bossEntity]->ongoingLevel == CurrentLevel::LEVEL1)
                bossSpawns[bossEntity]->ongoingLevel = CurrentLevel::LEVEL1_BOSS;
            if (bossSpawns[bossEntity]->ongoingLevel == CurrentLevel::LEVEL2)
                bossSpawns[bossEntity]->ongoingLevel = CurrentLevel::LEVEL2_BOSS;
        }
    }
}

void ecs::system::BossSpawnSystem::changeSpriteLevel2Boss(Registry &reg, SparseArray<component::Type> &types)
{
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    SparseArray<component::BossSpawn> &bossSpawns = reg.getComponents<component::BossSpawn>();
    std::size_t bossEntity = findChosenEntity(types, ecs::EntityType::BOSS);
    std::size_t bossLevel2Index = findChosenEntity(types, ecs::EntityType::BOSS_LEVEL2_TEMPLATE);
    std::size_t bossShotTemplate = findChosenEntity(types, ecs::EntityType::BOSS_SHOT_2_TEMPLATE);

    if (bossLevel2Index == 0 || bossShotTemplate == 0 || bossEntity == 0)
        return;
    if (bossEntity < bossSpawns.size() && bossEntity < spriteInfos.size() && bossLevel2Index < spriteInfos.size()) {
        if (!bossSpawns[bossEntity] || !spriteInfos[bossEntity] || !spriteInfos[bossLevel2Index])
            return;
        if (bossSpawns[bossEntity]->ongoingLevel == CurrentLevel::LEVEL2_BOSS) {
            spriteInfos[bossEntity]->spritePath = spriteInfos[bossLevel2Index]->spritePath;
            spriteInfos[bossEntity]->isLoaded = spriteInfos[bossLevel2Index]->isLoaded;
            spriteInfos[bossEntity]->patternNb = spriteInfos[bossLevel2Index]->patternNb;
            spriteInfos[bossEntity]->scaleFactor = spriteInfos[bossLevel2Index]->scaleFactor;
        }
    }
}

void ecs::system::BossSpawnSystem::createNewBoss(Registry &reg, SparseArray<component::Type> &types, const std::size_t &i) {
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::ShootingRate> &shootingRate = reg.getComponents<component::ShootingRate>();
    SparseArray<component::SoundEffectInfos> &soundEffect = reg.getComponents<component::SoundEffectInfos>();
    SparseArray<component::MovementPattern> &movementPatterns = reg.getComponents<component::MovementPattern>();

    entity_t entity = reg.spawnEntity();

    positions.insertAt(entity, ecs::component::Position{LEVEL1_BOSS_SPAWN_X, LEVEL1_BOSS_SPAWN_Y});
    velocities.insertAt(entity, ecs::component::Velocity{LEVEL1_BOSS_VELOCITY, 0, 0});
    spriteInfos.insertAt(entity, ecs::component::SpriteInfos{spriteInfos[i]->spritePath, spriteInfos[i]->patternNb,
                                                             spriteInfos[i]->firstPattern, spriteInfos[i]->scaleFactor});
    types.insertAt(entity, ecs::component::Type{ecs::EntityType::BOSS});
    lives.insertAt(entity, ecs::component::Life{lives[i]->nbLives, lives[i]->displayLife, lives[i]->hasToSummonPowerupAndWeapon});
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

std::size_t ecs::system::BossSpawnSystem::findChosenEntity(SparseArray<component::Type> &types, const ecs::EntityType &entityToFind)
{
    for (std::size_t i = 0; i < types.size(); i++) {
        if (types[i]->entityType == entityToFind)
            return i;
    }
    return 0;
}
