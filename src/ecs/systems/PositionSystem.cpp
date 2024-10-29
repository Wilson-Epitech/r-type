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
            if (!handlePlayerPosition(i, reg)) {
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

void ecs::system::PositionSystem::handleMovementPattern(Registry &reg, const std::size_t &index) {
    SparseArray<component::MovementPatternType> &movementPatternType = reg.getComponents<component::MovementPatternType>();
    SparseArray<component::MovementPattern> &movementPattern = reg.getComponents<component::MovementPattern>();

    if (movementPatternType.size() < index || movementPattern.size() < index) {
        moveStraight(reg, index);
        return;
    }
    if (!movementPatternType[index] || !movementPattern[index]) {
        moveStraight(reg, index);
        return;
    }
    switch (movementPatternType[index]->movementPatternType) {
        case ecs::MovementType::STRAIGHT:
            moveStraight(reg, index);
            break;
        case ecs::MovementType::SNAKE:
            moveSnake(reg, index);
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

    if (vy < 0)
        vy *= -1;
    if (positions[index]->y >= positions[index]->startY + velocities[index]->vx * movementPattern[index]->patternAmplitude && velocities[index]->vy < 0) {
        velocities[index]->vy = -vy;
    } else if (positions[index]->y <= positions[index]->startY - velocities[index]->vx * movementPattern[index]->patternAmplitude && velocities[index]->vy >= 0) {
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

void ecs::system::PositionSystem::setStartPosition(Registry &reg, const std::size_t &index) {
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();

    if (positions[index]->startX == 0)
        positions[index]->startX = positions[index]->x;
    if (positions[index]->startY == 0)
        positions[index]->startY = positions[index]->y;
}
