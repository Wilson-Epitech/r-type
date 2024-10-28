/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** ResetForNextLevel
*/

#include "ResetForNextLevel.hpp"

#include <iostream>

void ecs::system::ResetForNextLevel::operator()(Registry &reg)
{
    SparseArray<component::BossSpawn> &bossLevel = reg.getComponents<component::BossSpawn>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    SparseArray<component::Position> &position = reg.getComponents<component::Position>();
    SparseArray<component::Velocity> &velocity = reg.getComponents<component::Velocity>();
    SparseArray<component::SpriteInfos> &spriteInfo = reg.getComponents<component::SpriteInfos>();
    SparseArray<component::Drawable> &drawableBackground = reg.getComponents<component::Drawable>();

    for (std::size_t i = 0; i < bossLevel.size() && i < types.size(); i++) {
        if (bossLevel[i]->hasBeenSlayed && types[i]->entityType == ecs::EntityType::BOSS_LEVEL1) {

            for (std::size_t y = 0; y < position.size(); y++) {
                if (types[y]->entityType == ecs::EntityType::PLAYER) {
                    position[y]->x = 100;
                    position[y]->y = 400;
                    velocity[y]->vx = 0;
                    velocity[y]->vy = 0;
                }
            }

            for (std::size_t y = 0; y < types.size(); y++) {
                if (types[y]->entityType == ecs::EntityType::ENEMY || types[y]->entityType == ecs::EntityType::ENEMYSHOT)
                    reg.killEntity(y);
            }

            for (std::size_t y = 0; y < types.size(); y++) {
                if (types[y]->entityType == ecs::EntityType::BACKGROUND) {
                    spriteInfo[y]->spritePath = "assets/background_level2.png";
                    drawableBackground[y]->isLoaded = false;
                }
            }
        }
    }
}
