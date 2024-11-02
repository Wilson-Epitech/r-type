/*
** R_TYPE PROJECT, 2024
** mirror_playerWeapon
** File description:
** WeaponSystem
*/

#include "WeaponSystem.hpp"

void ecs::system::WeaponSystem::operator()(Registry &reg) {
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    SparseArray<component::Weapon> &weapons = reg.getComponents<component::Weapon>();

    for (std::size_t i = 0; i < lives.size() && i < types.size(); ++i) {
        if (lives[i] && types[i]) {
            if (lives[i]->nbLives == 0) {
                if (lives[i]->hasToSummonPowerup)
                    summonWeapon(reg, i);
            }
        }
    }
    for (std::size_t i = 0; i < weapons.size(); ++i) {
        if (!weapons[i])
            continue;
        if (!weapons[i]->hasBeenTaken)
            continue;
        activateWeapon(reg, i);
        desactivateWeapon(reg, i);
    }
}

void ecs::system::WeaponSystem::desactivateWeapon(Registry &reg, const std::size_t &index) {
    SparseArray<component::Weapon> &weapons = reg.getComponents<component::Weapon>();

    if (!weapons[index]->isActivated)
        return;

    if (Lib::isTimeElapsed(weapons[index]->spawnTime, weapons[index]->activeTime))
        reg.killEntity(index);
}

void ecs::system::WeaponSystem::activateWeapon(Registry &reg, const std::size_t &index) {
    SparseArray<component::Weapon> &weapons = reg.getComponents<component::Weapon>();

    if (weapons[index]->isActivated)
        return;
    weapons[index]->isActivated = true;
    weapons[index]->spawnTime = std::chrono::steady_clock::now();

    for (std::size_t i = 0; i < weapons.size(); ++i) {
        if (!weapons[i])
            continue;
        if (i == index)
            continue;
        if (weapons[i]->playerIndex == weapons[index]->playerIndex && weapons[i]->weaponType == weapons[index]->weaponType && weapons[i]->isActivated) {
            weapons[i]->spawnTime = std::chrono::steady_clock::now();
            reg.killEntity(index);
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
    ecs::WeaponType weaponToSpawn = (ecs::WeaponType)(std::rand() % (ecs::WeaponType::END_WEAPON - 1) + 1);
    std::size_t weaponIndex = getWeaponIndex(reg);
    std::size_t weaponSpriteIndex = getWeaponSpriteIndex(reg, weaponToSpawn);
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
    weapons.insertAt(entity, ecs::component::Weapon{weaponToSpawn, weapons[weaponIndex]->spawnRate, weapons[weaponIndex]->activeTime});
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
