/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** CollisionSystem
*/

#include "CollisionSystem.hpp"

void ecs::system::CollisionSystem::operator()(Registry &reg) {
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::SpriteSize> &spritesSizes = reg.getComponents<component::SpriteSize>();
    SparseArray<component::SpriteInfos> &spritesInfos = reg.getComponents<component::SpriteInfos>();
    Rectangle rec1 = Rectangle{};
    Rectangle rec2 = Rectangle{};

    fillSpriteSize(spritesInfos, spritesSizes);
    for (std::size_t i = 0;
         i < positions.size() && i < spritesInfos.size() && i < types.size() && i < lives.size() && i < spritesSizes.size(); ++i) {
        if (!positions[i] || !spritesInfos[i] || !types[i] || !lives[i] || !spritesSizes[i])
            continue;
        if (lives[i]->nbLives == 0)
            continue;
        rec1 = spriteHitbox(positions, spritesSizes, i);
        if (types[i] && positions[i]) {
            if ((types[i]->entityType == ecs::EntityType::PLAYERSHOT && !Lib::isOnScreen({positions[i]->x, positions[i]->y})) ||
                (types[i]->entityType == ecs::EntityType::ENEMY && positions[i]->x < 0))
                reg.killEntity(i);
        }
        for (std::size_t j = i;
             j < positions.size() && j < spritesInfos.size() && j < types.size() && j < lives.size() && j < spritesSizes.size(); ++j) {
            if (!positions[j] || !spritesInfos[j] || !types[j] || !lives[j] || !spritesSizes[j])
                continue;
            if (i == j || types[i]->entityType == types[j]->entityType || lives[j]->nbLives == 0)
                continue;
            rec2 = spriteHitbox(positions, spritesSizes, j);
            if (areEntitiesEnemies(types[i]->entityType, types[j]->entityType) && positions[i]->x > 0 &&
                positions[i]->x < Lib::getScreenWidth() && positions[j]->x > 0 && positions[j]->x < Lib::getScreenWidth()) {
                if (CheckCollisionRecs(rec1, rec2)) {
                    if (!lives[i]->isInvincible)
                        lives[i]->nbLives--;
                    if (!lives[j]->isInvincible)
                        lives[j]->nbLives--;
                    powerUpLives(reg, i, j);
                    reg.addCollision(i, j);
                }
            }
        }
    }
}

void ecs::system::CollisionSystem::fillSpriteSize(SparseArray<component::SpriteInfos> &spritesInfos,
                                                  SparseArray<component::SpriteSize> &spritesSizes) {

    Image tmpImage;

    for (std::size_t i = 0; i < spritesInfos.size(); i++) {
        if (!spritesInfos[i])
            continue;
        if (!spritesSizes[i] && spritesInfos[i]->spritePath.length() > 0) {
            spritesSizes.insertAt(i, ecs::component::SpriteSize{});
            if (!FileExists(spritesInfos[i]->spritePath.c_str()))
                throw Error("Invalid sprite path");
            tmpImage = LoadImage(spritesInfos[i]->spritePath.c_str());
            spritesSizes[i]->spriteWidth = (float(tmpImage.width) / spritesInfos[i]->patternNb) * spritesInfos[i]->scaleFactor;
            spritesSizes[i]->spriteHeight = float(tmpImage.height) * spritesInfos[i]->scaleFactor;
            UnloadImage(tmpImage);
        }
    }
}

Rectangle ecs::system::CollisionSystem::spriteHitbox(SparseArray<component::Position> &positions,
                                                     SparseArray<component::SpriteSize> &spritesSizes, std::size_t index) {
    return Rectangle{positions[index]->x, positions[index]->y, spritesSizes[index]->spriteWidth, spritesSizes[index]->spriteHeight};
}

void ecs::system::CollisionSystem::powerUpLives(Registry &reg, const std::size_t &i, const std::size_t &j)
{
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    SparseArray<component::Powerup> &powerup = reg.getComponents<component::Powerup>();
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();

    if (types[i]->entityType == ecs::EntityType::PLAYER && types[j]->entityType == ecs::EntityType::POWERUP && !lives[i]->isInvincible) {
        powerup[j]->playerIndex = i;
        lives[i]->nbLives++;
    } else if (types[i]->entityType == ecs::EntityType::POWERUP && types[j]->entityType == ecs::EntityType::PLAYER && !lives[j]->isInvincible) {
        powerup[i]->playerIndex = j;
        lives[j]->nbLives++;
    }
}

bool ecs::system::CollisionSystem::areEntitiesEnemies(const ecs::EntityType &firstType, const ecs::EntityType &secondType) {
    return (!isPlayerAndPlayerShot(firstType, secondType) && !isBackground(firstType, secondType) &&
            !isEnemyAndEnemyShot(firstType, secondType) && !isPlayerShotAndEnemyShot(firstType, secondType) && !isEnemyAndPowerup(firstType, secondType) &&
            !isEnemyShotAndPowerup(firstType, secondType) && ! isPlayerShotAndPowerup(firstType, secondType));
}

bool ecs::system::CollisionSystem::isPlayerAndPlayerShot(const ecs::EntityType &firstType, const ecs::EntityType &secondType) {
    return ((firstType == ecs::EntityType::PLAYER && secondType == ecs::EntityType::PLAYERSHOT) ||
            (firstType == ecs::EntityType::PLAYERSHOT && secondType == ecs::EntityType::PLAYER));
}

bool ecs::system::CollisionSystem::isEnemyAndEnemyShot(const ecs::EntityType &firstType, const ecs::EntityType &secondType) {
    return ((firstType == ecs::EntityType::ENEMY && secondType == ecs::EntityType::ENEMYSHOT) ||
            (firstType == ecs::EntityType::ENEMYSHOT && secondType == ecs::EntityType::ENEMY));
}

bool ecs::system::CollisionSystem::isPlayerShotAndEnemyShot(const ecs::EntityType &firstType, const ecs::EntityType &secondType) {
    return ((firstType == ecs::EntityType::PLAYERSHOT && secondType == ecs::EntityType::ENEMYSHOT) ||
            (firstType == ecs::EntityType::ENEMYSHOT && secondType == ecs::EntityType::PLAYERSHOT));
}

bool ecs::system::CollisionSystem::isBackground(const ecs::EntityType &firstType, const ecs::EntityType &secondType) {
    return ((firstType == ecs::EntityType::BACKGROUND || secondType == ecs::EntityType::BACKGROUND));
}

bool ecs::system::CollisionSystem::isEnemyAndPowerup(const ecs::EntityType &firstType, const ecs::EntityType &secondType) {
    return ((firstType == ecs::EntityType::ENEMY && secondType == ecs::EntityType::POWERUP) ||
            (firstType == ecs::EntityType::POWERUP && secondType == ecs::EntityType::ENEMY));
}

bool ecs::system::CollisionSystem::isEnemyShotAndPowerup(const ecs::EntityType &firstType, const ecs::EntityType &secondType) {
    return ((firstType == ecs::EntityType::ENEMYSHOT && secondType == ecs::EntityType::POWERUP) ||
            (firstType == ecs::EntityType::POWERUP && secondType == ecs::EntityType::ENEMYSHOT));
}

bool ecs::system::CollisionSystem::isPlayerShotAndPowerup(const ecs::EntityType &firstType, const ecs::EntityType &secondType) {
    return ((firstType == ecs::EntityType::PLAYERSHOT && secondType == ecs::EntityType::POWERUP) ||
            (firstType == ecs::EntityType::POWERUP && secondType == ecs::EntityType::PLAYERSHOT));
}

std::size_t ecs::system::CollisionSystem::getPowerupIndex(Registry &reg) {
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    SparseArray<component::Powerup> &powerup = reg.getComponents<component::Powerup>();

    for (std::size_t i = 0; i < types.size() && i < powerup.size(); ++i) {
        if (types[i] && powerup[i]) {
            if (types[i]->entityType == ecs::EntityType::POWERUP && !powerup[i]->hasBeenActivated) {
                return i;
            }
        }
    }
    return 0;
}