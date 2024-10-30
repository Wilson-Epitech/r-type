/*
** R_TYPE PROJECT, 2024
** branch
** File description:
** EnemyFiringSystem
*/

#include "EnemyFiringSystem.hpp"

void ecs::system::EnemyFiringSystem::operator()(Registry &reg) {
    SparseArray<component::ShootingRate> &enemyShootingRate = reg.getComponents<component::ShootingRate>();
    SparseArray<component::Type> &type = reg.getComponents<component::Type>();
    std::chrono::steady_clock::time_point time0 = std::chrono::steady_clock::now();
    static std::vector<std::pair<std::size_t, std::chrono::steady_clock::time_point>> timer;
    bool isTimerSet = false;

    for (std::size_t i = 0; i < type.size() && i < enemyShootingRate.size(); ++i) {
        if (type[i] && enemyShootingRate[i]) {
            if (type[i]->entityType != ecs::EntityType::ENEMY)
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
            if (Lib::isTimeElapsed(time0, enemyShootingRate[i]->shootRate)) {
                addShot(i, reg);
                for (std::size_t j = 0; j < timer.size(); j++) {
                    if (timer[j].first == i) {
                        timer[j].second = std::chrono::steady_clock::now();
                    }
                }
            }
        }
        isTimerSet = false;
    }
}

void ecs::system::EnemyFiringSystem::addShot(const std::size_t &enemyIndex, Registry &reg) {
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    SparseArray<component::SoundEffectInfos> &soundEffectInfos = reg.getComponents<component::SoundEffectInfos>();
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    std::size_t enemyShootIndex = getEnemyShootIndex(reg);

    if (enemyShootIndex == 0 || spriteInfos[enemyIndex]->spriteWidth == 0 || spriteInfos[enemyIndex]->spriteHeight == 0 ||
        spriteInfos[enemyShootIndex]->spriteWidth == 0 || spriteInfos[enemyShootIndex]->spriteHeight == 0)
        return;
    ecs::CustomRectangle shotSpriteRect = {0, 0, float(spriteInfos[enemyShootIndex]->spriteWidth),
                                           float(spriteInfos[enemyShootIndex]->spriteHeight)};
    float shotX = positions[enemyIndex]->x - (spriteInfos[enemyIndex]->spriteWidth + (shotSpriteRect.width / 2));
    float shotY = positions[enemyIndex]->y + ((spriteInfos[enemyIndex]->spriteHeight / 2 - shotSpriteRect.height / 2));
    float shootAngle = 0;
    entity_t entity = reg.spawnEntity();

    if (velocities[enemyIndex]->vx < 0) {
        if (velocities[enemyShootIndex]->vx > 0)
            velocities[enemyShootIndex]->vx *= -1;
    } else if (velocities[enemyIndex]->vx > 0) {
        shotX = positions[enemyIndex]->x + (spriteInfos[enemyIndex]->spriteWidth + (shotSpriteRect.width / 2));
        if (velocities[enemyShootIndex]->vx < 0)
            velocities[enemyShootIndex]->vx *= -1;
    }
    positions.insertAt(entity, ecs::component::Position{shotX, shotY});
    velocities.insertAt(entity, ecs::component::Velocity{velocities[enemyShootIndex]->vx, velocities[enemyShootIndex]->vy,
                                                         velocities[enemyShootIndex]->possibleVelocity});
    spriteInfos.insertAt(
        entity, ecs::component::SpriteInfos{spriteInfos[enemyShootIndex]->spritePath, spriteInfos[enemyShootIndex]->patternNb,
                                            spriteInfos[enemyShootIndex]->firstPattern, spriteInfos[enemyShootIndex]->scaleFactor});
    types.insertAt(entity, ecs::component::Type{ecs::EntityType::ENEMYSHOT});
    lives.insertAt(entity, ecs::component::Life{1});
    shootAngle = changeShootVelocity(reg, enemyIndex, entity);
    spriteInfos[entity]->rotationAngle = shootAngle;

    if (!soundEffectInfos[enemyShootIndex])
        return;
    if (soundEffectInfos[enemyShootIndex]->soundPath.length() > 0)
        soundEffectInfos.insertAt(entity, ecs::component::SoundEffectInfos{soundEffectInfos[enemyShootIndex]->soundPath, false, true});
}

std::size_t ecs::system::EnemyFiringSystem::getEnemyShootIndex(Registry &reg) {
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();

    for (std::size_t i = 0; i < spriteInfos.size() && i < velocities.size() && i < types.size(); ++i) {
        if (spriteInfos[i] && velocities[i] && types[i]) {
            if (spriteInfos[i]->spritePath.length() > 0 && types[i]->entityType == ecs::EntityType::ENEMYSHOT_TEMPLATE) {
                return i;
            }
        }
    }
    return 0;
}

float ecs::system::EnemyFiringSystem::changeShootVelocity(Registry &reg, const std::size_t &enemyIndex, const std::size_t &shootIndex) {
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    SparseArray<component::MovementPattern> &movementPattern = reg.getComponents<component::MovementPattern>();
    std::size_t playerEntity = findClosestPlayer(reg, enemyIndex);
    float speed = 0;
    float angle = 0;

    if (movementPattern.size() < enemyIndex || playerEntity == 0)
        return 0;
    if (!movementPattern[enemyIndex])
        return 0;
    if (movementPattern[enemyIndex]->movementPatternType != ecs::MovementType::FOLLOW)
        return 0;

    if (velocities[shootIndex]->vy < 0)
        speed += velocities[shootIndex]->vy * -1;
    else
        speed += velocities[shootIndex]->vy;
    if (velocities[shootIndex]->vx < 0)
        speed += velocities[shootIndex]->vx * -1;
    else
        speed += velocities[shootIndex]->vx;

    angle = atan2f(positions[playerEntity]->y - positions[enemyIndex]->y, positions[playerEntity]->x - positions[enemyIndex]->x);
    velocities[shootIndex]->vx = speed * cosf(angle);
    velocities[shootIndex]->vy = speed * sinf(angle);
    return (angle * 180 / M_PI) + 180;
}

std::size_t ecs::system::EnemyFiringSystem::findClosestPlayer(Registry &reg, const std::size_t &index) {
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