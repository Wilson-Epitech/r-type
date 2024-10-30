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
    SparseArray<component::SpriteInfos> &spritesInfos = reg.getComponents<component::SpriteInfos>();
    ecs::CustomRectangle rec1 = ecs::CustomRectangle{};
    ecs::CustomRectangle rec2 = ecs::CustomRectangle{};

    retrieveSpritesSizes(spritesInfos);
    for (std::size_t i = 0; i < positions.size() && i < spritesInfos.size() && i < types.size() && i < lives.size(); ++i) {
        if (!positions[i] || !spritesInfos[i] || !types[i] || !lives[i])
            continue;
        if (lives[i]->nbLives == 0)
            continue;
        rec1 = spriteHitbox(positions, spritesInfos, i);
        if (rec1.width == 0 || rec1.height == 0)
            continue;
        if (types[i] && positions[i]) {
            if ((types[i]->entityType == ecs::EntityType::PLAYERSHOT && !Lib::isOnScreen({positions[i]->x, positions[i]->y})) ||
                (types[i]->entityType == ecs::EntityType::ENEMY && positions[i]->x < 0))
                reg.killEntity(i);
        }
        for (std::size_t j = i; j < positions.size() && j < spritesInfos.size() && j < types.size() && j < lives.size(); ++j) {
            if (!positions[j] || !spritesInfos[j] || !types[j] || !lives[j])
                continue;
            if (i == j || types[i]->entityType == types[j]->entityType || lives[j]->nbLives == 0)
                continue;
            rec2 = spriteHitbox(positions, spritesInfos, j);
            if (rec2.width == 0 || rec2.height == 0)
                continue;
            if (areEntitiesEnemies(types[i]->entityType, types[j]->entityType) && positions[i]->x > 0 &&
                positions[i]->x < Lib::getScreenWidth() && positions[j]->x > 0 && positions[j]->x < Lib::getScreenWidth()) {
                if (areEntitiesInCollision(rec1, rec2)) {
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

void ecs::system::CollisionSystem::retrieveSpritesSizes(SparseArray<component::SpriteInfos> &spritesInfos) {
    unsigned int tmpWidth, tmpHeight = 0;

    for (std::size_t i = 0; i < spritesInfos.size(); ++i) {
        if (!spritesInfos[i])
            continue;
        if (spritesInfos[i]->spritePath.length() > 0 && spritesInfos[i]->spriteWidth == 0 && spritesInfos[i]->spriteHeight == 0) {
            std::ifstream spritePath(spritesInfos[i]->spritePath);
            spritePath.seekg(BYTES_OFFSET);
            spritePath.read((char *)&tmpWidth, LENGTH_IN_BYTES);
            spritePath.read((char *)&tmpHeight, LENGTH_IN_BYTES);
            tmpWidth = ntohl(tmpWidth);
            tmpHeight = ntohl(tmpHeight);
            spritesInfos[i]->spriteWidth = (tmpWidth / spritesInfos[i]->patternNb) * spritesInfos[i]->scaleFactor;
            spritesInfos[i]->spriteHeight = tmpHeight * spritesInfos[i]->scaleFactor;
        }
    }
}

ecs::CustomRectangle ecs::system::CollisionSystem::spriteHitbox(SparseArray<component::Position> &positions,
                                                                SparseArray<component::SpriteInfos> &spritesInfos, std::size_t index) {
    return ecs::CustomRectangle{positions[index]->x, positions[index]->y, float(spritesInfos[index]->spriteWidth),
                                float(spritesInfos[index]->spriteHeight)};
}

bool ecs::system::CollisionSystem::areEntitiesInCollision(ecs::CustomRectangle rec1, ecs::CustomRectangle rec2) {
    return (rec1.x <= rec2.x + rec2.width && rec1.x + rec1.width >= rec2.x && rec1.y <= rec2.y + rec2.height &&
            rec1.y + rec1.height >= rec2.y);
}

void ecs::system::CollisionSystem::powerUpLives(Registry &reg, const std::size_t &i, const std::size_t &j) {
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    SparseArray<component::Powerup> &powerup = reg.getComponents<component::Powerup>();
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();

    if (types[i]->entityType == ecs::EntityType::PLAYER && types[j]->entityType == ecs::EntityType::POWERUP && !lives[i]->isInvincible) {
        powerup[j]->playerIndex = i;
        lives[i]->nbLives++;
    } else if (types[i]->entityType == ecs::EntityType::POWERUP && types[j]->entityType == ecs::EntityType::PLAYER &&
               !lives[j]->isInvincible) {
        powerup[i]->playerIndex = j;
        lives[j]->nbLives++;
    }
}

bool ecs::system::CollisionSystem::areEntitiesEnemies(const ecs::EntityType &firstType, const ecs::EntityType &secondType) {
    return (!isPlayerAndPlayerShot(firstType, secondType) && !isBackground(firstType, secondType) &&
            !isEnemyAndEnemyShot(firstType, secondType) && !isPlayerShotAndEnemyShot(firstType, secondType) &&
            !isEnemyAndPowerup(firstType, secondType) && !isEnemyShotAndPowerup(firstType, secondType) &&
            !isPlayerShotAndPowerup(firstType, secondType) && !isBossLevel1AndBossLevel1Shot(firstType, secondType) &&
            !isBossLevel1AndEnemyShot(firstType, secondType) && !isBossLevel1AndEnemy(firstType, secondType) &&
            !isBossLevel1ShotAndPowerUp(firstType, secondType) && !isBossLevel1ShotAndPlayerShot(firstType, secondType));
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

bool ecs::system::CollisionSystem::isBossLevel1AndBossLevel1Shot(const ecs::EntityType &firstType, const ecs::EntityType &secondType) {
    return ((firstType == ecs::EntityType::BOSS_SHOT_1 && secondType == ecs::EntityType::BOSS_LEVEL1) ||
            (firstType == ecs::EntityType::BOSS_LEVEL1 && secondType == ecs::EntityType::BOSS_SHOT_1));
}

bool ecs::system::CollisionSystem::isBossLevel1AndEnemyShot(const ecs::EntityType &firstType, const ecs::EntityType &secondType) {
    return ((firstType == ecs::EntityType::ENEMYSHOT && secondType == ecs::EntityType::BOSS_LEVEL1) ||
            (firstType == ecs::EntityType::BOSS_LEVEL1 && secondType == ecs::EntityType::ENEMYSHOT));
}

bool ecs::system::CollisionSystem::isBossLevel1AndEnemy(const ecs::EntityType &firstType, const ecs::EntityType &secondType) {
    return ((firstType == ecs::EntityType::ENEMY && secondType == ecs::EntityType::BOSS_LEVEL1) ||
            (firstType == ecs::EntityType::BOSS_LEVEL1 && secondType == ecs::EntityType::ENEMY));
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

bool ecs::system::CollisionSystem::isBossLevel1ShotAndPowerUp(const ecs::EntityType &firstType, const ecs::EntityType &secondType) {
    return ((firstType == ecs::EntityType::BOSS_SHOT_1 && secondType == ecs::EntityType::POWERUP) ||
            (firstType == ecs::EntityType::POWERUP && secondType == ecs::EntityType::BOSS_SHOT_1));
}

bool ecs::system::CollisionSystem::isBossLevel1ShotAndPlayerShot(const ecs::EntityType &firstType, const ecs::EntityType &secondType) {
    return ((firstType == ecs::EntityType::BOSS_SHOT_1 && secondType == ecs::EntityType::PLAYERSHOT) ||
            (firstType == ecs::EntityType::PLAYERSHOT && secondType == ecs::EntityType::BOSS_SHOT_1));
}