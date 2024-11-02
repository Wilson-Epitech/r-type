/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** PositionSystem
*/

#include "PositionSystem.hpp"

void ecs::system::PositionSystem::operator()(Registry &reg) {
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();

    for (std::size_t i = 0; i < positions.size() && i < velocities.size(); ++i) {
        if (positions[i] && velocities[i]) {
            setStartPosition(reg, i);
            if (!handlePlayerPosition(i, reg) && !handleWeaponPosition(i, reg)) {
                handleMovementPattern(reg, i);
            }
        }
    }
}

bool ecs::system::PositionSystem::handlePlayerPosition(const std::size_t &playerIndex, Registry &reg) {
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    ecs::CustomVector2D pos = {positions[playerIndex]->x + velocities[playerIndex]->vx,
                               positions[playerIndex]->y + velocities[playerIndex]->vy};

    if (spriteInfos[playerIndex] && types[playerIndex]) {
        if (types[playerIndex]->entityType == ecs::EntityType::PLAYER) {
            pos.x += spriteInfos[playerIndex]->spriteWidth / 2;
            pos.y += spriteInfos[playerIndex]->spriteHeight / 2;
            if (Lib::isOnScreen(pos)) {
                positions[playerIndex]->x += velocities[playerIndex]->vx;
                positions[playerIndex]->y += velocities[playerIndex]->vy;
            }
            return true;
        }
    }
    return false;
}

bool ecs::system::PositionSystem::handleWeaponPosition(const std::size_t &weaponIndex, Registry &reg) {
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Weapon> &weapons = reg.getComponents<component::Weapon>();
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    float x = 0;
    float y = 0;

    if (!weapons[weaponIndex])
        return false;
    if (!positions[weapons[weaponIndex]->playerIndex] || !spriteInfos[weapons[weaponIndex]->playerIndex])
        return false;
    if (weapons[weaponIndex]->weaponType == ecs::WeaponType::FRONT_WEAPON) {
        x += (float)spriteInfos[weapons[weaponIndex]->playerIndex]->spriteWidth / spriteInfos[weapons[weaponIndex]->playerIndex]->scaleFactor;
        x += spriteInfos[weaponIndex]->spriteWidth / 2;
    }
    if (weapons[weaponIndex]->weaponType == ecs::WeaponType::BACK_WEAPON) {
        x -= (float)spriteInfos[weapons[weaponIndex]->playerIndex]->spriteWidth / spriteInfos[weapons[weaponIndex]->playerIndex]->scaleFactor;
        x -= spriteInfos[weaponIndex]->spriteWidth / 2;
    }
    y += ((float)spriteInfos[weapons[weaponIndex]->playerIndex]->spriteHeight / 2) / spriteInfos[weapons[weaponIndex]->playerIndex]->scaleFactor;
    positions[weaponIndex]->x = positions[weapons[weaponIndex]->playerIndex]->x + x;
    positions[weaponIndex]->y = positions[weapons[weaponIndex]->playerIndex]->y + y;
    return true;
}

void ecs::system::PositionSystem::handleMovementPattern(Registry &reg, const std::size_t &index) {
    SparseArray<component::MovementPattern> &movementPattern = reg.getComponents<component::MovementPattern>();

    if (movementPattern.size() < index) {
        moveStraight(reg, index);
        return;
    }
    if (!movementPattern[index]) {
        moveStraight(reg, index);
        return;
    }
    handleReverseDirection(reg, index);
    switch (movementPattern[index]->movementPatternType) {
    case ecs::MovementType::STRAIGHT:
        moveStraight(reg, index);
        break;
    case ecs::MovementType::SNAKE:
        moveSnake(reg, index);
        break;
    case ecs::MovementType::FOLLOW:
        moveFollow(reg, index);
        break;
    default:
        break;
    }
}

void ecs::system::PositionSystem::moveStraight(Registry &reg, const std::size_t &index) {
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();

    positions[index]->x += velocities[index]->vx;
    positions[index]->y += velocities[index]->vy;
}

void ecs::system::PositionSystem::moveSnake(Registry &reg, const std::size_t &index) {
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    SparseArray<component::MovementPattern> &movementPattern = reg.getComponents<component::MovementPattern>();
    float vy = velocities[index]->vy;
    float vx = velocities[index]->vx;

    if (vy < 0)
        vy *= -1;
    if (vx > 0)
        vx *= -1;
    if (positions[index]->y >= positions[index]->startY + vx * movementPattern[index]->patternAmplitude && velocities[index]->vy < 0) {
        velocities[index]->vy = -vy;
    } else if (positions[index]->y <= positions[index]->startY - vx * movementPattern[index]->patternAmplitude &&
               velocities[index]->vy >= 0) {
        velocities[index]->vy = vy;
    } else if (velocities[index]->vy >= 0) {
        velocities[index]->vy = -vy;
    } else if (velocities[index]->vy <= 0) {
        velocities[index]->vy = vy;
    }
    if (positions[index]->y <= 0)
        velocities[index]->vy = -vy;
    if (positions[index]->y >= Lib::getScreenHeight())
        velocities[index]->vy = vy;
    positions[index]->x += velocities[index]->vx;
    positions[index]->y += velocities[index]->vy;
}

void ecs::system::PositionSystem::moveFollow(Registry &reg, const std::size_t &index) {
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    std::size_t playerEntity = findClosestPlayer(reg, index);
    float speed = 0;
    float angle = 0;
    float vx = 0;
    float vy = 0;

    if (playerEntity == 0) {
        positions[index]->x += velocities[index]->vx;
        positions[index]->y += velocities[index]->vy;
        return;
    }
    if (velocities[index]->vy < 0)
        speed += velocities[index]->vy * -1;
    else
        speed += velocities[index]->vy;
    if (velocities[index]->vx < 0)
        speed += velocities[index]->vx * -1;
    else
        speed += velocities[index]->vx;

    angle = atan2f(positions[playerEntity]->y - positions[index]->y, positions[playerEntity]->x - positions[index]->x);
    vx = speed * cosf(angle);
    vy = speed * sinf(angle);

    positions[index]->x += vx;
    positions[index]->y += vy;
}

void ecs::system::PositionSystem::setStartPosition(Registry &reg, const std::size_t &index) {
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();

    if (positions[index]->startX == 0)
        positions[index]->startX = positions[index]->x;
    if (positions[index]->startY == 0)
        positions[index]->startY = positions[index]->y;
}

std::size_t ecs::system::PositionSystem::findClosestPlayer(Registry &reg, const std::size_t &index) {
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    float tmpRes = 0;
    float lastPlayerDistance = 0;
    std::size_t playerIndex = 0;

    for (std::size_t i = 0; i < types.size() && i < positions.size(); ++i) {
        if (!types[i] || !positions[i])
            continue;
        if (types[i]->entityType == ecs::EntityType::PLAYER) {
            tmpRes = (positions[index]->x + positions[index]->y) - (positions[i]->x + positions[i]->y);
            if (tmpRes < 0) {
                tmpRes *= -1;
            }
            if (tmpRes < lastPlayerDistance || lastPlayerDistance == 0) {
                lastPlayerDistance = tmpRes;
                playerIndex = i;
            }
        }
    }
    return playerIndex;
}

void ecs::system::PositionSystem::handleReverseDirection(Registry &reg, const std::size_t &index) {
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    SparseArray<component::MovementPattern> &movementPattern = reg.getComponents<component::MovementPattern>();

    if (!movementPattern[index]->isReversible)
        return;
    if (positions[index]->x <= 0)
        velocities[index]->vx *= -1;
    if (positions[index]->x >= Lib::getScreenWidth() && velocities[index]->vx > 0)
        velocities[index]->vx *= -1;
}