/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** ResetForNextLevel
*/

#include "ResetForNextLevel.hpp"

void ecs::system::ResetForNextLevel::operator()(Registry &reg) {
    SparseArray<component::BossSpawn> &bossSpawns = reg.getComponents<component::BossSpawn>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();

    for (std::size_t i = 0; i < bossSpawns.size() && i < types.size(); i++) {
        if (bossSpawns[i]->hasBeenSlayed && types[i]->entityType == ecs::EntityType::BOSS_LEVEL1) {
            resetPlayerPosition(reg, types);
            killAllRemainingEnemies(reg, types);
            setNewLevelBackground(reg, types);
            changeSpriteForEnemies(reg, types);
        }
    }
}

void ecs::system::ResetForNextLevel::resetPlayerPosition(Registry &reg, SparseArray<component::Type> &types) {
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    std::vector<std::pair<entity_t, ecs::CustomColor>> playersColors = reg.getPlayersColors();
    std::vector<std::pair<ecs::CustomColor, ecs::component::Position>> spaceshipColorPositionCorrespondence =
        reg.getSpaceshipColorPositionCorrespondence();

    for (std::size_t y = 0; y < positions.size(); y++) {
        if (types[y]->entityType == ecs::EntityType::PLAYER) {
            auto itPlayerColors =
                std::find_if(playersColors.begin(), playersColors.end(), [y](const auto &pair) { return ((pair.first == y)); });

            if (itPlayerColors != playersColors.end()) {
                auto itShipColors = std::find_if(spaceshipColorPositionCorrespondence.begin(), spaceshipColorPositionCorrespondence.end(),
                                                 [itPlayerColors](const auto &pair) { return ((pair.first == itPlayerColors->second)); });
                if (itShipColors != spaceshipColorPositionCorrespondence.end()) {
                    positions.insertAt(y, itShipColors->second);
                    velocities[y]->vx = 0;
                    velocities[y]->vy = 0;
                }
            }
        }
    }
}

void ecs::system::ResetForNextLevel::killAllRemainingEnemies(Registry &reg, SparseArray<component::Type> &types) {
    for (std::size_t y = 0; y < types.size(); y++) {
        if (types[y]->entityType == ecs::EntityType::ENEMY || types[y]->entityType == ecs::EntityType::ENEMYSHOT ||
            types[y]->entityType == ecs::EntityType::PLAYERSHOT || types[y]->entityType == ecs::EntityType::POWERUP)
            reg.killEntity(y);
    }
}

void ecs::system::ResetForNextLevel::setNewLevelBackground(Registry &reg, SparseArray<component::Type> &types) {
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();

    for (std::size_t i = 0; i < types.size(); i++) {
        if (!types[i])
            continue;
        if (types[i]->entityType == ecs::EntityType::BACKGROUND) {
            spriteInfos[i]->spritePath = LEVEL2_BACKGROUND_PATH;
            spriteInfos[i]->isLoaded = false;
        }
    }
}

void ecs::system::ResetForNextLevel::changeSpriteForEnemies(Registry &reg, SparseArray<component::Type> &types) {
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    SparseArray<component::MovementPattern> &movementPatterns = reg.getComponents<component::MovementPattern>();

    for (std::size_t i = 0; i < types.size(); i++) {
        if (!types[i])
            continue;
        if (types[i]->entityType == ecs::EntityType::ENEMY_TEMPLATE) {
            if (i < spriteInfos.size()) {
                if (spriteInfos[i]) {
                    spriteInfos[i]->spritePath = LEVEL2_ENEMY_PATH;
                    spriteInfos[i]->isLoaded = false;
                    spriteInfos[i]->scaleFactor = 3;
                    spriteInfos[i]->patternNb = 4;
                }
            }
            if (i < movementPatterns.size()) {
                if (movementPatterns[i])
                    movementPatterns[i]->movementPatternType = ecs::MovementType::FOLLOW;
            }
        }
    }
}
