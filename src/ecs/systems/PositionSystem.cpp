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
                handleMovementPatern(reg, i);
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

void ecs::system::PositionSystem::handleMovementPatern(Registry &reg, const std::size_t &index) {
    SparseArray<component::MovementPatern> &movementPatern = reg.getComponents<component::MovementPatern>();

    if (movementPatern.size() < index) {
        moveStraight(reg, index);
        return;
    }
    if (!movementPatern[index]) {
        moveStraight(reg, index);
        return;
    }
    switch (movementPatern[index]->movementPaternType) {
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

    if (positions[index]->y >= positions[index]->startY + velocities[index]->vx * 40 && velocities[index]->vy < 0) {
        velocities[index]->vy = -0.5;
    } else if (positions[index]->y <= positions[index]->startY - velocities[index]->vx * 40 && velocities[index]->vy >= 0) {
        velocities[index]->vy = 0.5;
    } else if (velocities[index]->vy >= 0) {
        velocities[index]->vy = -0.5;
    } else if (velocities[index]->vy <= 0) {
        velocities[index]->vy = 0.5;
    }
    if (positions[index]->y <= 0)
        velocities[index]->vy = -1;
    if (positions[index]->y >= Lib::getScreenHeight())
        velocities[index]->vy = 1;
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