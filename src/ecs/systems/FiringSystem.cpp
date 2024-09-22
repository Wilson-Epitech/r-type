/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** FiringSystem
*/

#include "FiringSystem.hpp"

void ecs::system::FiringSystem::operator()(Registry &reg)
{
    SparseArray<component::Controllable> &controllables = reg.getComponents<component::Controllable>();
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();

    for (std::size_t i = 0; i < controllables.size() && i < positions.size(); ++i) {
        if (controllables[i] && positions[i]) {
            if (!controllables[i]->isControllable)
                continue;
            if (IsKeyPressed(KEY_SPACE)) {
                addShot(reg.spawnEntity(), positions[i]->x, positions[i]->y, reg);
            }
        }
    }
}

void ecs::system::FiringSystem::addShot(entity_t entity, float x, float y, Registry &reg) {
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Drawable> &drawables = reg.getComponents<component::Drawable>();
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();

    positions.insertAt(entity, ecs::component::Position{x, y});
    velocities.insertAt(entity, ecs::component::Velocity{3.0f, 0});
    drawables.insertAt(entity, ecs::component::Drawable{PLAYERSHOT_PATH, PLAYERSHOT_SPRITE_NB_PATTERN});
    types.insertAt(entity, ecs::component::Type{ecs::entityType::PLAYERSHOT});
    lives.insertAt(entity, ecs::component::Life{1});
}
