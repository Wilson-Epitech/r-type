/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** BossFiringSystem
*/

#include "BossFiringSystem.hpp"

void ecs::system::BossFiringSystem::operator()(Registry &reg)
{
    SparseArray<component::Type> &type = reg.getComponents<component::Type>();
    SparseArray<component::ShootingRate> &enemyShootingRate = reg.getComponents<component::ShootingRate>();
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();

    std::chrono::steady_clock::time_point time0 = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point time1 = std::chrono::steady_clock::now();
    static std::vector<std::pair<std::size_t, std::chrono::steady_clock::time_point>> timer;
    std::chrono::milliseconds timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(time1 - time0);
    bool isTimerSet = false;

    for (std::size_t i = 0; i < type.size(); ++i) {
        if (type[i]->entityType != ecs::EntityType::BOSS_LEVEL1)
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
        if (timeElapsed.count() >= Lib::toMilliseconds(enemyShootingRate[i]->shootRate) && positions[i]->x == 1200) {
            addShot(i, reg);
            for (std::size_t j = 0; j < timer.size(); j++) {
                if (timer[j].first == i) {
                    timer[j].second = std::chrono::steady_clock::now();
                }
            }
        }
        isTimerSet = false;
    }
}

void ecs::system::BossFiringSystem::addShot(std::size_t enemyIndex, Registry &reg) {
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    SparseArray<component::SoundEffectInfos> &soundEffect = reg.getComponents<component::SoundEffectInfos>();
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    std::size_t enemyShootIndex = getBossShootIndex(reg);

    if (enemyShootIndex == 0 || spriteInfos[enemyIndex]->spriteWidth == 0 || spriteInfos[enemyIndex]->spriteHeight == 0 ||
        spriteInfos[enemyShootIndex]->spriteWidth == 0 || spriteInfos[enemyShootIndex]->spriteHeight == 0)
        return;
    float shotX = SHOT_POS_X;
    float shotY = (rand() % (MAX_SHOT_Y - MIN_SHOT_Y) + MIN_SHOT_Y);
    entity_t entity = reg.spawnEntity();

    positions.insertAt(entity, ecs::component::Position{shotX, shotY});
    velocities.insertAt(entity, ecs::component::Velocity{velocities[enemyShootIndex]->vx, velocities[enemyShootIndex]->vy,
                                                         velocities[enemyShootIndex]->possibleVelocity});
    spriteInfos.insertAt(
        entity, ecs::component::SpriteInfos{spriteInfos[enemyShootIndex]->spritePath, spriteInfos[enemyShootIndex]->patternNb,
                                            spriteInfos[enemyShootIndex]->firstPattern, spriteInfos[enemyShootIndex]->scaleFactor});
    types.insertAt(entity, ecs::component::Type{ecs::EntityType::BOSS_SHOT_1});
    lives.insertAt(entity, ecs::component::Life{1});

    if (!soundEffect[enemyShootIndex])
        return;
    if (soundEffect[enemyShootIndex]->soundPath.length() > 0)
        soundEffect.insertAt(entity, ecs::component::SoundEffectInfos{soundEffect[enemyShootIndex]->soundPath, false, true});
}

std::size_t ecs::system::BossFiringSystem::getBossShootIndex(Registry &reg) {
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

