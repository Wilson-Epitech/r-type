/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** FiringSystem
*/

#include "FiringSystem.hpp"

void ecs::system::FiringSystem::operator()(Registry &reg) {
    SparseArray<component::ShootingRate> &shootingRate = reg.getComponents<component::ShootingRate>();
    SparseArray<component::Shooting> &shootings = reg.getComponents<component::Shooting>();
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    std::chrono::steady_clock::time_point time0 = std::chrono::steady_clock::now();
    static std::vector<std::pair<std::size_t, std::chrono::steady_clock::time_point>> timer;
    std::chrono::steady_clock::time_point time1 = std::chrono::steady_clock::now();
    std::chrono::milliseconds timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(time1 - time0);
    bool isTimerSet = false;

    for (std::size_t i = 0; i < shootings.size() && i < positions.size() && i < shootingRate.size(); ++i) {
        if (shootings[i] && positions[i] && shootingRate[i]) {
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
            if (timeElapsed.count() >= Lib::toMilliseconds(shootingRate[i]->shootRate)) {
                if (shootings[i]->isShooting) {
                    addShot(i, reg, shootings[i]->shootPower, shootings[i]->shootSize);
                    shootings[i]->isShooting = false;
                }
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

void ecs::system::FiringSystem::addShot(std::size_t playerIndex, Registry &reg, std::size_t shootPower, float shootSize) {
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    SparseArray<component::SoundEffect> &soundEffect = reg.getComponents<component::SoundEffect>();
    SparseArray<component::SpriteSize> &spritesSizes = reg.getComponents<component::SpriteSize>();
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    SparseArray<component::ShootPlayerIndex> &shootPlayerIndex = reg.getComponents<component::ShootPlayerIndex>();
    std::size_t playerShootIndex = getPlayerShootIndex(reg);
    if (playerShootIndex == 0 || !spritesSizes[playerShootIndex])
        return;
    Image tmpShotImage = LoadImage(spriteInfos[playerShootIndex]->spritePath.c_str());
    float offset = float(tmpShotImage.width) / spriteInfos[playerShootIndex]->patternNb;
    Rectangle shotSpriteRect = {0, 0, offset, float(tmpShotImage.height)};
    entity_t entity = reg.spawnEntity();
    float shotX = positions[playerIndex]->x + (float((spritesSizes[playerIndex]->spriteWidth)) + ((shotSpriteRect.width) / 2));
    float shotY = positions[playerIndex]->y + (float((spritesSizes[playerIndex]->spriteHeight) / 2) - ((shotSpriteRect.height) / 2));
    UnloadImage(tmpShotImage);

    positions.insertAt(entity, ecs::component::Position{shotX, shotY});
    velocities.insertAt(entity, ecs::component::Velocity{velocities[playerShootIndex]->vx, velocities[playerShootIndex]->vy,
                                                         velocities[playerShootIndex]->possibleVelocity});
    spriteInfos.insertAt(
        entity, ecs::component::SpriteInfos{spriteInfos[playerShootIndex]->spritePath, spriteInfos[playerShootIndex]->patternNb,
                                            spriteInfos[playerShootIndex]->firstPattern, spriteInfos[playerShootIndex]->scaleFactor * shootSize});
    types.insertAt(entity, ecs::component::Type{ecs::EntityType::PLAYERSHOT});
    lives.insertAt(entity, ecs::component::Life{shootPower});
    shootPlayerIndex.insertAt(entity, ecs::component::ShootPlayerIndex{playerIndex});

    if (!soundEffect[playerShootIndex])
        return;
    if (soundEffect[playerShootIndex]->soundPath.length() > 0)
        soundEffect.insertAt(entity, ecs::component::SoundEffect{soundEffect[playerShootIndex]->soundPath, false, true});
}

std::size_t ecs::system::FiringSystem::getPlayerShootIndex(Registry &reg) {
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    SparseArray<component::Velocity> &playerShootVelocities = reg.getComponents<component::Velocity>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();

    for (std::size_t i = 0; i < spriteInfos.size() && i < playerShootVelocities.size() && i < types.size(); ++i) {
        if (spriteInfos[i] && playerShootVelocities[i] && types[i]) {
            if (spriteInfos[i]->spritePath.length() > 0 && types[i]->entityType == ecs::EntityType::PLAYERSHOT_TEMPLATE) {
                return i;
            }
        }
    }
    return 0;
}
