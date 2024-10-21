/*
** R_TYPE PROJECT, 2024
** powerup
** File description:
** PowerupSystem
*/

#include "PowerupSystem.hpp"

void ecs::system::PowerupSystem::operator()(Registry &reg) {
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();
    SparseArray<component::Type> &type = reg.getComponents<component::Type>();

    for (std::size_t i = 0; i < lives.size(); ++i) {
        if (lives[i]) {
            if (lives[i]->nbLives == 0) {
                if (lives[i]->hasToSummonPowerup)
                    summonPowerUp(reg, i);
                if (i < type.size()) {
                    if (type[i]) {
                        if (type[i]->entityType == ecs::EntityType::POWERUP)
                            usePowerup(reg, i);
                    }
                }
            }
        }
    }
}

void ecs::system::PowerupSystem::usePowerup(Registry &reg, const std::size_t index)
{
    SparseArray<component::Powerup> &powerup = reg.getComponents<component::Powerup>();
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();
    std::size_t powerupIndex = getPowerupIndex(reg);

    if (powerup.size() < index)
        return;
    if (!powerup[index])
        return;

    if (powerup[index]->powerupType == ecs::Poweruptype::LIFE_POWERUP)
        lives[powerup[powerupIndex]->playerIndex]->nbLives++;
    reg.killEntity(index);
}

void ecs::system::PowerupSystem::summonPowerUp(Registry &reg, const std::size_t index)
{
    SparseArray<component::Type> &type = reg.getComponents<component::Type>();
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    SparseArray<component::Drawable> &drawables = reg.getComponents<component::Drawable>();
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    SparseArray<component::Powerup> &powerup = reg.getComponents<component::Powerup>();
    std::size_t powerupIndex = getPowerupIndex(reg);
    entity_t entity = 0;

    if (type.size() < index || positions.size() < index || spriteInfos.size() < powerupIndex)
        return;
    if (!type[index] || !positions[index] || !spriteInfos[powerupIndex])
        return;
    if (rand() % powerup[powerupIndex]->spawnRate != 0) {
        return;
    }

    entity = reg.spawnEntity();
    positions.insertAt(entity, ecs::component::Position{positions[index]->x, positions[index]->y});
    velocities.insertAt(entity, ecs::component::Velocity{0, 0});
    spriteInfos.insertAt(entity, ecs::component::SpriteInfos{spriteInfos[powerupIndex]->spritePath, spriteInfos[powerupIndex]->patternNb, spriteInfos[powerupIndex]->firstPattern, spriteInfos[powerupIndex]->scaleFactor});
    drawables.insertAt(entity, ecs::component::Drawable{});
    types.insertAt(entity, ecs::component::Type{ecs::EntityType::POWERUP});
    lives.insertAt(entity, ecs::component::Life{1});
    powerup.insertAt(entity, ecs::component::Powerup{ecs::Poweruptype::LIFE_POWERUP, powerup[powerupIndex]->spawnRate, powerup[powerupIndex]->duration, true, powerup[powerupIndex]->playerIndex});
}

std::size_t ecs::system::PowerupSystem::getPowerupIndex(Registry &reg) {
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    SparseArray<component::Powerup> &powerup = reg.getComponents<component::Powerup>();

    for (std::size_t i = 0; i < types.size(); ++i) {
        if (types[i]) {
            if (types[i]->entityType == ecs::EntityType::POWERUP && !powerup[i]->hasBeenActivated) {
                return i;
            }
        }
    }
    return 0;
}
