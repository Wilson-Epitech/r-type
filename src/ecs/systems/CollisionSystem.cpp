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

    for (std::size_t i = 0;
         i < positions.size() && i < spritesInfos.size() && i < types.size() && i < lives.size(); ++i) {
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
        for (std::size_t j = i;
             j < positions.size() && j < spritesInfos.size() && j < types.size() && j < lives.size(); ++j) {
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
                    lives[i]->nbLives--;
                    lives[j]->nbLives--;
                    reg.addCollision(i, j);
                }
            }
        }
    }
}

ecs::CustomRectangle ecs::system::CollisionSystem::spriteHitbox(SparseArray<component::Position> &positions, SparseArray<component::SpriteInfos> &spritesInfos, std::size_t index) {
    return ecs::CustomRectangle{positions[index]->x, positions[index]->y, spritesInfos[index]->spriteWidth, spritesInfos[index]->spriteHeight};
}

bool ecs::system::CollisionSystem::areEntitiesInCollision(ecs::CustomRectangle rec1, ecs::CustomRectangle rec2) {
    return (rec1.x <= rec2.x + rec2.width && rec1.x + rec1.width >= rec2.x &&
    rec1.y <= rec2.y + rec2.height && rec1.y + rec1.height >= rec2.y);
}

bool ecs::system::CollisionSystem::areEntitiesEnemies(const ecs::EntityType &firstType, const ecs::EntityType &secondType) {
    return (!isPlayerAndPlayerShot(firstType, secondType) && !isBackground(firstType, secondType) &&
            !isEnemyAndEnemyShot(firstType, secondType) && !isPlayerShotAndEnemyShot(firstType, secondType));
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
