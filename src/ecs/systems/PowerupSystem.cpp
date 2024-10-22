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
    static std::vector<std::pair<std::size_t, std::chrono::steady_clock::time_point>> powerShootTimer;
    static std::vector<std::pair<std::size_t, std::chrono::steady_clock::time_point>> invincibleTimer;
    static std::vector<std::pair<std::size_t, std::chrono::steady_clock::time_point>> megaShootTimer;
    std::pair<std::size_t, ecs::Poweruptype> usedPowerup = {0, ecs::Poweruptype::NOPOWERUP};

    for (std::size_t i = 0; i < lives.size() && i < type.size(); ++i) {
        if (lives[i] && type[i]) {
            if (lives[i]->nbLives == 0) {
                if (lives[i]->hasToSummonPowerup)
                    summonPowerUp(reg, i);
                if (type[i]->entityType == ecs::EntityType::POWERUP)
                    usedPowerup = usePowerup(reg, i);
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
        }
    }
    clearPowerShootEffect(reg, powerShootTimer);
    clearInvicibleEffect(reg, invincibleTimer);
    clearMegaShootEffect(reg, megaShootTimer);
}

std::pair<std::size_t, ecs::Poweruptype> ecs::system::PowerupSystem::usePowerup(Registry &reg, const std::size_t index)
{
    SparseArray<component::Powerup> &powerup = reg.getComponents<component::Powerup>();
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();
    SparseArray<component::Shooting> &shootings = reg.getComponents<component::Shooting>();
    std::size_t powerupIndex = getPowerupIndex(reg);

    if (powerup.size() < index)
        return {0, ecs::Poweruptype::NOPOWERUP};
    if (!powerup[index])
        return {0, ecs::Poweruptype::NOPOWERUP};

    switch (powerup[index]->powerupType) {
        case ecs::Poweruptype::LIFE_POWERUP:
            lives[powerup[powerupIndex]->playerIndex]->nbLives++;
            return {powerup[powerupIndex]->playerIndex, ecs::Poweruptype::LIFE_POWERUP};
        case ecs::Poweruptype::POWERSHOOT_POWERUP:
            shootings[powerup[powerupIndex]->playerIndex]->shootPower = 3;
            updateActivePowerup(reg, powerup[powerupIndex]->playerIndex, POWERSHOOT_POWERUP, true);
            return {powerup[powerupIndex]->playerIndex, ecs::Poweruptype::POWERSHOOT_POWERUP};
        case ecs::Poweruptype::INVINCIBLE_POWERUP:
            lives[powerup[powerupIndex]->playerIndex]->isInvincible = true;
            updateActivePowerup(reg, powerup[powerupIndex]->playerIndex, INVINCIBLE_POWERUP, true);
            return {powerup[powerupIndex]->playerIndex, ecs::Poweruptype::INVINCIBLE_POWERUP};
        case ecs::Poweruptype::MEGASHOOT_POWERUP:
            shootings[powerup[powerupIndex]->playerIndex]->shootSize = 2;
            updateActivePowerup(reg, powerup[powerupIndex]->playerIndex, MEGASHOOT_POWERUP, true);
            return {powerup[powerupIndex]->playerIndex, ecs::Poweruptype::MEGASHOOT_POWERUP};
        default:
            break;
    }
    return {0, ecs::Poweruptype::NOPOWERUP};
}

void ecs::system::PowerupSystem::clearMegaShootEffect(Registry &reg, std::vector<std::pair<std::size_t, std::chrono::steady_clock::time_point>> &megaShootTimer) {
    SparseArray<component::Shooting> &shootings = reg.getComponents<component::Shooting>();

    for (std::size_t i = 0; i < megaShootTimer.size(); i++) {
        if (isTimeElapsed(megaShootTimer[i].second, EFFECT_DURATION)) {
            shootings[megaShootTimer[i].first]->shootSize = 1;
            updateActivePowerup(reg, megaShootTimer[i].first, MEGASHOOT_POWERUP, false);
            megaShootTimer.erase(megaShootTimer.begin() + i);
        }
    }
}

void ecs::system::PowerupSystem::clearPowerShootEffect(Registry &reg, std::vector<std::pair<std::size_t, std::chrono::steady_clock::time_point>> &powerShootTimer) {
    SparseArray<component::Shooting> &shootings = reg.getComponents<component::Shooting>();

    for (std::size_t i = 0; i < powerShootTimer.size(); i++) {
        if (isTimeElapsed(powerShootTimer[i].second, EFFECT_DURATION)) {
            shootings[powerShootTimer[i].first]->shootPower = 1;
            updateActivePowerup(reg, powerShootTimer[i].first, POWERSHOOT_POWERUP, false);
            powerShootTimer.erase(powerShootTimer.begin() + i);
        }
    }
}

void ecs::system::PowerupSystem::clearInvicibleEffect(Registry &reg, std::vector<std::pair<std::size_t, std::chrono::steady_clock::time_point>> &invincibleTimer) {
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();

    for (std::size_t i = 0; i < invincibleTimer.size(); i++) {
        if (isTimeElapsed(invincibleTimer[i].second, EFFECT_DURATION)) {
            lives[invincibleTimer[i].first]->isInvincible = false;
            updateActivePowerup(reg, invincibleTimer[i].first, INVINCIBLE_POWERUP, false);
            invincibleTimer.erase(invincibleTimer.begin() + i);
        }
    }
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
    ecs::Poweruptype powerupToSpawn = selectPowerup();

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
    spriteInfos.insertAt(entity, ecs::component::SpriteInfos{spriteInfos[powerupIndex].value()});
    drawables.insertAt(entity, ecs::component::Drawable{});
    types.insertAt(entity, ecs::component::Type{ecs::EntityType::POWERUP});
    lives.insertAt(entity, ecs::component::Life{1});
    powerup.insertAt(entity, ecs::component::Powerup{powerupToSpawn, powerup[powerupIndex]->spawnRate, powerup[powerupIndex]->duration, true, powerup[powerupIndex]->playerIndex});
}

std::size_t ecs::system::PowerupSystem::getPowerupIndex(Registry &reg) {
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    SparseArray<component::Powerup> &powerup = reg.getComponents<component::Powerup>();

    for (std::size_t i = 0; i < types.size() && i < powerup.size() ; ++i) {
        if (types[i] && powerup[i]) {
            if (types[i]->entityType == ecs::EntityType::POWERUP && !powerup[i]->hasBeenActivated) {
                return i;
            }
        }
    }
    return 0;
}

ecs::Poweruptype ecs::system::PowerupSystem::selectPowerup(void) {
    int randNumber = std::rand() % ecs::Poweruptype::NOPOWERUP;
    if (randNumber == ecs::Poweruptype::NOPOWERUP)
        randNumber--;
    return (ecs::Poweruptype)randNumber;
}

bool ecs::system::PowerupSystem::isTimeElapsed(std::chrono::steady_clock::time_point time, float duration) {
    std::chrono::steady_clock::time_point actualTime = std::chrono::steady_clock::now();
    std::chrono::milliseconds timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(actualTime - time);

    return timeElapsed.count() >= Lib::toMilliseconds(duration);
}

void ecs::system::PowerupSystem::updateActivePowerup(Registry &reg, std::size_t playerEntity, ecs::Poweruptype powerupType, bool isActive) {
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
