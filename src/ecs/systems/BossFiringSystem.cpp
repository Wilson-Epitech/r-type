/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** BossFiringSystem
*/

#include "BossFiringSystem.hpp"

void ecs::system::BossFiringSystem::operator()(Registry &reg)
{
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    SparseArray<component::ShootingRate> &shootingRate = reg.getComponents<component::ShootingRate>();
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();

    std::chrono::steady_clock::time_point time0 = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point time1 = std::chrono::steady_clock::now();
    static std::vector<std::pair<std::size_t, std::chrono::steady_clock::time_point>> timer;
    std::chrono::milliseconds timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(time1 - time0);
    bool isTimerSet = false;

    for (std::size_t i = 0; i < types.size() && i < shootingRate.size() && positions.size(); ++i) {
        if (!types[i] || !shootingRate[i] || !positions[i])
            continue;
        if (types[i]->entityType != ecs::EntityType::BOSS)
            continue;
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
        timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(time1 - time0);
        if (timeElapsed.count() >= Lib::toMilliseconds(shootingRate[i]->shootRate) && positions[i]->x <= SHOT_POS_X) {
            changeShootSprite(reg, types);
            addShot(i, reg);
            additionalShotFinalLevel(reg, types, i);
            for (std::size_t j = 0; j < timer.size(); j++) {
                if (timer[j].first == i) {
                    timer[j].second = std::chrono::steady_clock::now();
                }
            }
        }
        isTimerSet = false;
    }
}

void ecs::system::BossFiringSystem::changeShootSprite(Registry &reg, SparseArray<component::Type> types)
{
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    SparseArray<component::BossSpawn> &bossSpawn = reg.getComponents<component::BossSpawn>();
    SparseArray<component::MovementPattern> &movements = reg.getComponents<component::MovementPattern>();
    std::size_t bossIndex = findChosenBossEntity(types, ecs::EntityType::BOSS);
    std::size_t bossShotTemplateIndex = findChosenBossEntity(types, ecs::EntityType::BOSS_SHOT_2_TEMPLATE);
    std::size_t bossLevel2TemplateIndex = findChosenBossEntity(types, ecs::EntityType::BOSS_LEVEL2_TEMPLATE);
    std::size_t bossShotIndex = findChosenBossEntity(types, ecs::EntityType::BOSS_SHOT_1);
    static bool Level2BossShotLoaded = false;

    if (bossIndex < bossSpawn.size() && bossIndex < movements.size() && bossShotIndex < spriteInfos.size()) {
        if (!bossSpawn[bossIndex] || !spriteInfos[bossShotIndex] || !movements[bossIndex])
            return;
        if (bossSpawn[bossIndex]->ongoingLevel == CurrentLevel::LEVEL2_BOSS && !Level2BossShotLoaded) {
            spriteInfos[bossShotIndex]->spritePath = spriteInfos[bossShotTemplateIndex]->spritePath;
            spriteInfos[bossShotIndex]->isLoaded = spriteInfos[bossShotTemplateIndex]->isLoaded;
            spriteInfos[bossShotIndex]->patternNb = spriteInfos[bossShotTemplateIndex]->patternNb;
            movements[bossIndex]->movementPatternType = movements[bossLevel2TemplateIndex]->movementPatternType;
            Level2BossShotLoaded = true;
        }
    }
}

void ecs::system::BossFiringSystem::additionalShotFinalLevel(Registry &reg, SparseArray<component::Type> types, const std::size_t &i)
{
    SparseArray<component::BossSpawn> &bossSpawn = reg.getComponents<component::BossSpawn>();
    SparseArray<component::MovementPattern> &movements = reg.getComponents<component::MovementPattern>();

    std::size_t bossIndex = findChosenBossEntity(types, ecs::EntityType::BOSS);

    if (bossIndex < bossSpawn.size() && bossIndex < movements.size()) {
        if (!bossSpawn[bossIndex] || !movements[bossIndex])
            return;
        if (bossSpawn[bossIndex]->ongoingLevel == CurrentLevel::LEVEL3) {
            movements[bossIndex]->movementPatternType = ecs::MovementType::FOLLOW;
            addShot(i, reg);
            movements[bossIndex]->movementPatternType = ecs::MovementType::STRAIGHT;
        }
    }
}

void ecs::system::BossFiringSystem::addShot(const std::size_t &bossIndex, Registry &reg)
{
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    SparseArray<component::SoundEffectInfos> &soundEffect = reg.getComponents<component::SoundEffectInfos>();
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    std::size_t bossShootIndex = getBossShootIndex(reg);
    float shootAngle = 0;

    if (bossShootIndex == 0 || spriteInfos[bossIndex]->spriteWidth == 0 || spriteInfos[bossIndex]->spriteHeight == 0 ||
        spriteInfos[bossShootIndex]->spriteWidth == 0 || spriteInfos[bossShootIndex]->spriteHeight == 0)
        return;
    float shotX = SHOT_POS_X;
    float shotY = (rand() % (MAX_SHOT_Y - MIN_SHOT_Y) + MIN_SHOT_Y);
    entity_t entity = reg.spawnEntity();

    positions.insertAt(entity, ecs::component::Position{shotX, shotY});
    velocities.insertAt(entity, ecs::component::Velocity{velocities[bossShootIndex]->vx, velocities[bossShootIndex]->vy,
                                                         velocities[bossShootIndex]->possibleVelocity});
    spriteInfos.insertAt(entity,
                         ecs::component::SpriteInfos{spriteInfos[bossShootIndex]->spritePath, spriteInfos[bossShootIndex]->patternNb,
                                                     spriteInfos[bossShootIndex]->firstPattern, spriteInfos[bossShootIndex]->scaleFactor});
    types.insertAt(entity, ecs::component::Type{ecs::EntityType::BOSS_SHOT_1});
    lives.insertAt(entity, ecs::component::Life{1});
    shootAngle = changeShootVelocity(reg, findChosenBossEntity(types, ecs::EntityType::BOSS), entity);
    spriteInfos[entity]->rotationAngle = shootAngle;

    if (!soundEffect[bossShootIndex])
        return;
    if (soundEffect[bossShootIndex]->soundPath.length() > 0)
        soundEffect.insertAt(entity, ecs::component::SoundEffectInfos{soundEffect[bossShootIndex]->soundPath, false, true});
}

std::size_t ecs::system::BossFiringSystem::getBossShootIndex(Registry &reg)
{
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    SparseArray<component::Velocity> &enemyShootVelocities = reg.getComponents<component::Velocity>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();

    for (std::size_t i = 0; i < spriteInfos.size() && i < enemyShootVelocities.size() && i < types.size(); ++i) {
        if (spriteInfos[i] && enemyShootVelocities[i] && types[i]) {
            if (spriteInfos[i]->spritePath.length() > 0 && types[i]->entityType == ecs::EntityType::BOSS_SHOT_1_TEMPLATE) {
                return i;
            }
        }
    }
    return 0;
}

std::size_t ecs::system::BossFiringSystem::findChosenBossEntity(SparseArray<component::Type> &types, const ecs::EntityType &entityToFind)
{
    for (std::size_t i = 0; i < types.size(); i++) {
        if (types[i]->entityType == entityToFind)
            return i;
    }
    return 0;
}

float ecs::system::BossFiringSystem::changeShootVelocity(Registry &reg, const std::size_t &bossIndex, const std::size_t &shootIndex)
{
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    SparseArray<component::MovementPattern> &movementPattern = reg.getComponents<component::MovementPattern>();
    std::size_t playerEntity = findClosestPlayer(reg, bossIndex);
    float speed = 0;
    float angle = 0;

    if (movementPattern.size() < bossIndex || playerEntity == 0)
        return 0;
    if (!movementPattern[bossIndex])
        return 0;
    if (movementPattern[bossIndex]->movementPatternType != ecs::MovementType::FOLLOW)
        return 0;

    if (velocities[shootIndex]->vy < 0)
        speed += velocities[shootIndex]->vy * -1;
    else
        speed += velocities[shootIndex]->vy;
    if (velocities[shootIndex]->vx < 0)
        speed += velocities[shootIndex]->vx * -1;
    else
        speed += velocities[shootIndex]->vx;

    angle = atan2f(positions[playerEntity]->y - positions[shootIndex]->y, positions[playerEntity]->x - positions[shootIndex]->x);
    velocities[shootIndex]->vx = speed * cosf(angle);
    velocities[shootIndex]->vy = speed * sinf(angle);
    return (angle * 180 / M_PI) + 180;
}

std::size_t ecs::system::BossFiringSystem::findClosestPlayer(Registry &reg, const std::size_t &index)
{
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    float tmpRes = 0;
    float lastPlayerDistance = 0;
    std::size_t playerIndex = 0;

    for (std::size_t i = 0; i < types.size() && i < positions.size(); ++i) {
        if (!types[i] || !positions[i])
            continue;
        if (types[i]->entityType == ecs::EntityType::PLAYER) {
            tmpRes = (positions[index]->x + positions[index]->y) - (positions[i]->x + positions[i]->y);
            if (tmpRes < 0) {
                tmpRes *= -1;
            }
            if (tmpRes < lastPlayerDistance || lastPlayerDistance == 0) {
                lastPlayerDistance = tmpRes;
                playerIndex = i;
            }
        }
    }
    return playerIndex;
}