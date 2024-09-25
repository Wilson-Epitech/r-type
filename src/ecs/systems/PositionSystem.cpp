/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** PositionSystem
*/

#include "PositionSystem.hpp"

void ecs::system::PositionSystem::operator()(Registry &reg)
{
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    SparseArray<component::Type> &type = reg.getComponents<component::Type>();

    for (std::size_t i = 0; i < positions.size() && i < velocities.size(); ++i) {
        if (positions[i] && velocities[i]) {
            positions[i]->x += velocities[i]->vx;
            positions[i]->y += velocities[i]->vy;
        }
        if (type[i] && positions[i]) {
            if (type[i]->type == ecs::EntityType::PLAYERSHOT && !Lib::isOnScreen({positions[i]->x, positions[i]->y}))
                reg.killEntity(i);
        }
    }
}
