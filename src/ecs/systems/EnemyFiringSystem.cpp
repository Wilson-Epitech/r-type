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
    std::chrono::_V2::steady_clock::time_point time0 = std::chrono::steady_clock::now();
    static std::vector<std::pair<std::size_t, std::chrono::_V2::steady_clock::time_point>> timer;
    std::chrono::_V2::steady_clock::time_point time1 = std::chrono::steady_clock::now();
    std::chrono::milliseconds timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(time1 - time0);
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
            timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(time1 - time0);
            if (timeElapsed.count() >= Lib::toMilliseconds(enemyShootingRate[i]->shootRate)) {
                addShot(i, reg);
            }
            if (timeElapsed.count() >= Lib::toMilliseconds(enemyShootingRate[i]->shootRate)) {
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

void ecs::system::EnemyFiringSystem::addShot(std::size_t enemyIndex, Registry &reg) {
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    SparseArray<component::Drawable> &drawables = reg.getComponents<component::Drawable>();
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    std::size_t enemyShootIndex = getEnemyShootIndex(reg);
    if (enemyShootIndex == 0)
        return;
    entity_t entity = reg.spawnEntity();
    Texture2D shotTexture = LoadTexture(spriteInfos[enemyShootIndex]->spritePath.c_str());
    float offset = float(shotTexture.width) / spriteInfos[enemyShootIndex]->patternNb;
    Rectangle shotSpriteRect = {0, 0, offset, float(shotTexture.height)};
    float shotX = positions[enemyIndex]->x - (float(drawables[enemyIndex]->spriteRect.width) / 2 - float(shotSpriteRect.width) / 2);
    float shotY = positions[enemyIndex]->y + ((float(drawables[enemyIndex]->spriteRect.height) / 2) * spriteInfos[enemyIndex]->scaleFactor -
                                              (float(shotSpriteRect.height) / 2) * spriteInfos[enemyShootIndex]->scaleFactor);
    UnloadTexture(shotTexture);

    // soundHasToBePlayed(reg);
    positions.insertAt(entity, ecs::component::Position{shotX, shotY});
    velocities.insertAt(entity, ecs::component::Velocity{velocities[enemyShootIndex]->vx, velocities[enemyShootIndex]->vy,
                                                         velocities[enemyShootIndex]->possibleVelocity});
    spriteInfos.insertAt(
        entity, ecs::component::SpriteInfos{spriteInfos[enemyShootIndex]->spritePath, spriteInfos[enemyShootIndex]->patternNb,
                                            spriteInfos[enemyShootIndex]->firstPattern, spriteInfos[enemyShootIndex]->scaleFactor});
    drawables.insertAt(entity, ecs::component::Drawable{});
    types.insertAt(entity, ecs::component::Type{ecs::EntityType::ENEMYSHOT});
    lives.insertAt(entity, ecs::component::Life{1});
}

std::size_t ecs::system::EnemyFiringSystem::getEnemyShootIndex(Registry &reg) {
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    SparseArray<component::Velocity> &enemyShootVelocities = reg.getComponents<component::Velocity>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();

    for (std::size_t i = 0; i < spriteInfos.size() && i < enemyShootVelocities.size() && i < types.size(); ++i) {
        if (spriteInfos[i] && enemyShootVelocities[i] && types[i]) {
            if (spriteInfos[i]->spritePath.length() > 0 && types[i]->entityType == ecs::EntityType::ENEMYSHOT_TEMPLATE) {
                return i;
            }
        }
    }
    return 0;
}

void ecs::system::EnemyFiringSystem::soundHasToBePlayed(Registry &reg) {
    SparseArray<component::SoundEffect> &soundEffect = reg.getComponents<component::SoundEffect>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();

    for (std::size_t i = 0; i < types.size() && i < soundEffect.size(); ++i) {
        if (types[i] && soundEffect[i]) {
            if (types[i]->entityType == ecs::EntityType::ENEMYSHOT) {
                soundEffect[i]->hasToBePlayed = true;
            }
        }
    }
}
