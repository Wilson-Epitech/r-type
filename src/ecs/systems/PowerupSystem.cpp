/*
** R_TYPE PROJECT, 2024
** powerup
** File description:
** PowerupSystem
*/

#include "PowerupSystem.hpp"

void ecs::system::PowerupSystem::operator()(Registry &reg) {
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    static std::vector<std::pair<std::size_t, std::chrono::steady_clock::time_point>> powerShootTimer;
    static std::vector<std::pair<std::size_t, std::chrono::steady_clock::time_point>> invincibleTimer;
    static std::vector<std::pair<std::size_t, std::chrono::steady_clock::time_point>> megaShootTimer;
    std::pair<std::size_t, ecs::Poweruptype> usedPowerup = {0, ecs::Poweruptype::NOPOWERUP};

    for (std::size_t i = 0; i < lives.size() && i < types.size(); ++i) {
        if (lives[i] && types[i]) {
            if (lives[i]->nbLives == 0) {
                if (lives[i]->hasToSummonPowerupAndWeapon)
                    summonPowerUp(reg, i);
                if (types[i]->entityType == ecs::EntityType::POWERUP)
                    usedPowerup = usePowerup(reg, i, lives);
            }
        }
        if (usedPowerup.second != ecs::Poweruptype::NOPOWERUP) {
            switch (usedPowerup.second) {
            case ecs::Poweruptype::POWERSHOOT_POWERUP:
                powerShootTimer.push_back({usedPowerup.first, std::chrono::steady_clock::now()});
                break;
            case ecs::Poweruptype::INVINCIBLE_POWERUP:
                invincibleTimer.push_back({usedPowerup.first, std::chrono::steady_clock::now()});
                break;
            case ecs::Poweruptype::MEGASHOOT_POWERUP:
                megaShootTimer.push_back({usedPowerup.first, std::chrono::steady_clock::now()});
                break;
            default:
                break;
            }
            updateActivePowerup(reg, usedPowerup.first, usedPowerup.second, true);
        }
        usedPowerup = {0, ecs::Poweruptype::NOPOWERUP};
    }
    clearPowerShootEffect(reg, powerShootTimer);
    clearInvicibleEffect(reg, invincibleTimer);
    clearMegaShootEffect(reg, megaShootTimer);
}

std::pair<std::size_t, ecs::Poweruptype> ecs::system::PowerupSystem::usePowerup(Registry &reg, const std::size_t &index,
                                                                                SparseArray<component::Life> &lives) {
    SparseArray<component::Powerup> &powerup = reg.getComponents<component::Powerup>();
    SparseArray<component::Shooting> &shootings = reg.getComponents<component::Shooting>();
    std::size_t playerIndex = 0;

    if (powerup.size() < index)
        return {0, ecs::Poweruptype::NOPOWERUP};
    if (!powerup[index])
        return {0, ecs::Poweruptype::NOPOWERUP};

    playerIndex = powerup[index]->playerIndex;

    if (shootings.size() < playerIndex)
        return {0, ecs::Poweruptype::NOPOWERUP};
    if (!shootings[playerIndex])
        return {0, ecs::Poweruptype::NOPOWERUP};

    switch (powerup[index]->powerupType) {
    case ecs::Poweruptype::LIFE_POWERUP:
        lives[playerIndex]->nbLives++;
        return {playerIndex, ecs::Poweruptype::LIFE_POWERUP};
    case ecs::Poweruptype::POWERSHOOT_POWERUP:
        shootings[playerIndex]->shootPower = 3;
        return {playerIndex, ecs::Poweruptype::POWERSHOOT_POWERUP};
    case ecs::Poweruptype::INVINCIBLE_POWERUP:
        lives[playerIndex]->isInvincible = true;
        return {playerIndex, ecs::Poweruptype::INVINCIBLE_POWERUP};
    case ecs::Poweruptype::MEGASHOOT_POWERUP:
        shootings[playerIndex]->shootSize = 2;
        return {playerIndex, ecs::Poweruptype::MEGASHOOT_POWERUP};
    default:
        break;
    }
    return {0, ecs::Poweruptype::NOPOWERUP};
}

void ecs::system::PowerupSystem::clearMegaShootEffect(
    Registry &reg, std::vector<std::pair<std::size_t, std::chrono::steady_clock::time_point>> &megaShootTimer) {
    SparseArray<component::Shooting> &shootings = reg.getComponents<component::Shooting>();

    for (std::size_t i = 0; i < megaShootTimer.size(); i++) {
        if (Lib::isTimeElapsed(megaShootTimer[i].second, EFFECT_DURATION)) {
            if (megaShootTimer.size() > 1) {
                megaShootTimer.erase(megaShootTimer.begin() + i);
                i--;
                continue;
            }
            shootings[megaShootTimer[i].first]->shootSize = 1;
            updateActivePowerup(reg, megaShootTimer[i].first, MEGASHOOT_POWERUP, false);
            megaShootTimer.erase(megaShootTimer.begin() + i);
            i--;
        }
    }
}

void ecs::system::PowerupSystem::clearPowerShootEffect(
    Registry &reg, std::vector<std::pair<std::size_t, std::chrono::steady_clock::time_point>> &powerShootTimer) {
    SparseArray<component::Shooting> &shootings = reg.getComponents<component::Shooting>();

    for (std::size_t i = 0; i < powerShootTimer.size(); i++) {
        if (Lib::isTimeElapsed(powerShootTimer[i].second, EFFECT_DURATION)) {
            if (powerShootTimer.size() > 1) {
                powerShootTimer.erase(powerShootTimer.begin() + i);
                i--;
                continue;
            }
            shootings[powerShootTimer[i].first]->shootPower = 1;
            updateActivePowerup(reg, powerShootTimer[i].first, POWERSHOOT_POWERUP, false);
            powerShootTimer.erase(powerShootTimer.begin() + i);
            i--;
        }
    }
}

void ecs::system::PowerupSystem::clearInvicibleEffect(
    Registry &reg, std::vector<std::pair<std::size_t, std::chrono::steady_clock::time_point>> &invincibleTimer) {
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();

    for (std::size_t i = 0; i < invincibleTimer.size(); i++) {
        if (Lib::isTimeElapsed(invincibleTimer[i].second, EFFECT_DURATION)) {
            if (invincibleTimer.size() > 1) {
                invincibleTimer.erase(invincibleTimer.begin() + i);
                i--;
                continue;
            }
            lives[invincibleTimer[i].first]->isInvincible = false;
            updateActivePowerup(reg, invincibleTimer[i].first, INVINCIBLE_POWERUP, false);
            invincibleTimer.erase(invincibleTimer.begin() + i);
            i--;
        }
    }
}

void ecs::system::PowerupSystem::summonPowerUp(Registry &reg, const std::size_t &index) {
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    SparseArray<component::Powerup> &powerup = reg.getComponents<component::Powerup>();
    std::size_t powerupIndex = getPowerupIndex(reg);
    entity_t entity = 0;
    ecs::Poweruptype powerupToSpawn = (ecs::Poweruptype)(std::rand() % (ecs::Poweruptype::END_POWERUP_TYPE - 1) + 1);

    if (types.size() < index || positions.size() < index || spriteInfos.size() < powerupIndex)
        return;
    if (!types[index] || !positions[index] || !spriteInfos[powerupIndex])
        return;
    if (rand() % powerup[powerupIndex]->spawnRate != 0) {
        return;
    }

    entity = reg.spawnEntity();
    positions.insertAt(entity, ecs::component::Position{positions[index]->x, positions[index]->y});
    velocities.insertAt(entity, ecs::component::Velocity{});
    spriteInfos.insertAt(entity, ecs::component::SpriteInfos{spriteInfos[powerupIndex].value()});
    types.insertAt(entity, ecs::component::Type{ecs::EntityType::POWERUP});
    lives.insertAt(entity, ecs::component::Life{});
    powerup.insertAt(entity,
                     ecs::component::Powerup{powerupToSpawn, powerup[powerupIndex]->spawnRate, true, powerup[powerupIndex]->playerIndex});
}

std::size_t ecs::system::PowerupSystem::getPowerupIndex(Registry &reg) {
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();

    for (std::size_t i = 0; i < types.size(); ++i) {
        if (types[i]) {
            if (types[i]->entityType == ecs::EntityType::POWERUP_TEMPLATE) {
                return i;
            }
        }
    }
    return 0;
}

void ecs::system::PowerupSystem::updateActivePowerup(Registry &reg, const std::size_t &playerEntity, const ecs::Poweruptype &powerupType,
                                                     const bool &isActive) {
    SparseArray<component::ActivePowerup> &activePowerup = reg.getComponents<component::ActivePowerup>();

    if (!activePowerup[playerEntity])
        return;
    switch (powerupType) {
    case ecs::Poweruptype::POWERSHOOT_POWERUP:
        activePowerup[playerEntity]->isPowershootActive = isActive;
        break;
    case ecs::Poweruptype::INVINCIBLE_POWERUP:
        activePowerup[playerEntity]->isInvincibleActive = isActive;
        break;
    case ecs::Poweruptype::MEGASHOOT_POWERUP:
        activePowerup[playerEntity]->isMegaShootActive = isActive;
        break;
    default:
        break;
    }
}
