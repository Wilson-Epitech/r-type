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
            if (!handlePlayerPosition(i, reg)) {
                positions[i]->x += velocities[i]->vx;
                positions[i]->y += velocities[i]->vy;
            }
        }
    }
}

bool ecs::system::PositionSystem::handlePlayerPosition(std::size_t playerIndex, Registry &reg) {
    // SparseArray<ecs::component::Drawable> drawables;
    // SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    // SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    // SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    // ecs::CustomVector2D pos = {positions[playerIndex]->x + velocities[playerIndex]->vx, positions[playerIndex]->y + velocities[playerIndex]->vy};

    // try {
    //     drawables = reg.getComponents<component::Drawable>();
    // } catch (const std::exception &e) {
    // }

    // if (drawables[playerIndex] && types[playerIndex]) {
    //     if (types[playerIndex]->entityType == ecs::EntityType::PLAYER) {
    //         pos.x += drawables[playerIndex]->spriteRect.width / 2;
    //         pos.y += drawables[playerIndex]->spriteRect.height / 2;
    //         if (Lib::isOnScreen(pos)) {
    //             positions[playerIndex]->x += velocities[playerIndex]->vx;
    //             positions[playerIndex]->y += velocities[playerIndex]->vy;
    //         }
    //         return true;
    //     }
    // }
    return false;
}
