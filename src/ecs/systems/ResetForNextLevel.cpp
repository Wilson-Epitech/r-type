/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** ResetForNextLevel
*/

#include "ResetForNextLevel.hpp"

void ecs::system::ResetForNextLevel::operator()(Registry &reg)
{
    SparseArray<component::BossSpawn> &bossLevel = reg.getComponents<component::BossSpawn>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();

    for (std::size_t i = 0; i < bossLevel.size() && i < types.size(); i++) {
        if (bossLevel[i]->hasBeenSlayed && types[i]->entityType == ecs::EntityType::BOSS_LEVEL1) {
            resetPlayerPosition(reg, types);
            killAllRemainingEnemies(reg, types);
            setNewLevelBackground(reg, types);
        }
    }
}

void ecs::system::ResetForNextLevel::resetPlayerPosition(Registry &reg, SparseArray<component::Type> &types)
{
    SparseArray<component::Position> &position = reg.getComponents<component::Position>();
    SparseArray<component::Velocity> &velocity = reg.getComponents<component::Velocity>();

    for (std::size_t y = 0; y < position.size(); y++) {
        if (types[y]->entityType == ecs::EntityType::PLAYER) {
            position[y]->x = RESET_POS_PLAYER_X;
            position[y]->y = RESET_POS_PLAYER_Y;
            velocity[y]->vx = 0;
            velocity[y]->vy = 0;
        }
    }
}

void ecs::system::ResetForNextLevel::killAllRemainingEnemies(Registry &reg, SparseArray<component::Type> &types)
{
    for (std::size_t y = 0; y < types.size(); y++) {
        if (types[y]->entityType == ecs::EntityType::ENEMY || types[y]->entityType == ecs::EntityType::ENEMYSHOT ||
            types[y]->entityType == ecs::EntityType::PLAYERSHOT || types[y]->entityType == ecs::EntityType::POWERUP)
            reg.killEntity(y);
    }
}

void ecs::system::ResetForNextLevel::setNewLevelBackground(Registry &reg, SparseArray<component::Type> &types)
{
    SparseArray<component::SpriteInfos> &spriteInfo = reg.getComponents<component::SpriteInfos>();

    for (std::size_t y = 0; y < types.size(); y++) {
        if (types[y]->entityType == ecs::EntityType::BACKGROUND) {
            spriteInfo[y]->spritePath = LEVEL2_BACKGROUND_PATH;
            spriteInfo[y]->isLoaded = false;
        }
    }
}
