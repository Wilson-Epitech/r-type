/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** KillOffscreenEntitiesSystem
*/

#include "KillOffscreenEntitiesSystem.hpp"

void ecs::system::KillOffscreenEntitiesSystem::operator()(Registry &reg) {
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();

    for (std::size_t i = 0; i < positions.size() && i < types.size(); ++i) {
        if (types[i] && positions[i]) {
            if ((types[i]->type == ecs::EntityType::PLAYERSHOT && !Lib::isOnScreen({positions[i]->x, positions[i]->y})) ||
                (types[i]->type == ecs::EntityType::ENEMYSHOT && !Lib::isOnScreen({positions[i]->x, positions[i]->y})) ||
                (types[i]->type == ecs::EntityType::ENEMY && positions[i]->x < 0))
                reg.killEntity(i);
        }
    }
}
