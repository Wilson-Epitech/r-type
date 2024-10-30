/*
** R_TYPE PROJECT, 2024
** mirror_playerWeapon
** File description:
** WeaponSystem
*/

#include "WeaponSystem.hpp"
#include <iostream>

void ecs::system::WeaponSystem::operator()(Registry &reg) {
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();

    for (std::size_t i = 0; i < lives.size() && i < types.size(); ++i) {
        if (lives[i] && types[i]) {
            if (lives[i]->nbLives == 0) {
                if (lives[i]->hasToSummonPowerup)
                    summonWeapon(reg, i);
            }
        }
    }
}

void ecs::system::WeaponSystem::summonWeapon(Registry &reg, const std::size_t &index)
{
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    SparseArray<component::Weapon> &weapons = reg.getComponents<component::Weapon>();
    ecs::WeaponType powerupToSpawn = (ecs::WeaponType)(std::rand() % (ecs::WeaponType::END_WEAPON - 1) + 1);
    std::size_t weaponIndex = getWeaponIndex(reg);
    std::size_t weaponSpriteIndex = getWeaponSpriteIndex(reg, powerupToSpawn);
    entity_t entity = 0;

    if (types.size() < index || positions.size() < index || spriteInfos.size() < weaponSpriteIndex || weaponSpriteIndex == 0)
        return;
    if (!types[index] || !positions[index] || !spriteInfos[weaponSpriteIndex])
        return;
    if (rand() % weapons[weaponIndex]->spawnRate != 0) {
        return;
    }
    entity = reg.spawnEntity();
    positions.insertAt(entity, ecs::component::Position{positions[index]->x, positions[index]->y});
    velocities.insertAt(entity, ecs::component::Velocity{});
    spriteInfos.insertAt(entity, ecs::component::SpriteInfos{spriteInfos[weaponSpriteIndex].value()});
    types.insertAt(entity, ecs::component::Type{ecs::EntityType::WEAPON});
    lives.insertAt(entity, ecs::component::Life{});
    weapons.insertAt(entity, ecs::component::Weapon{powerupToSpawn, weapons[weaponIndex]->spawnRate, true, weapons[weaponIndex]->playerIndex});
}

std::size_t ecs::system::WeaponSystem::getWeaponIndex(Registry &reg) {
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();

    for (std::size_t i = 0; i < types.size(); ++i) {
        if (types[i]) {
            if (types[i]->entityType == ecs::EntityType::WEAPON_TEMPLATE) {
                return i;
            }
        }
    }
    return 0;
}

std::size_t ecs::system::WeaponSystem::getWeaponSpriteIndex(Registry &reg, ecs::WeaponType weaponType) {
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    SparseArray<component::Weapon> &weapons = reg.getComponents<component::Weapon>();

    for (std::size_t i = 0; i < types.size() && i < weapons.size(); ++i) {
        if (types[i] && weapons[i]) {
            if (types[i]->entityType == ecs::EntityType::WEAPON_SPRITES) {
                if (weapons[i]->weaponType == weaponType)
                    return i;
            }
        }
    }
    return 0;
}
