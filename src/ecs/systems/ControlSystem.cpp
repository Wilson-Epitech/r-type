/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** ControlSystem
*/

#include "ControlSystem.hpp"

void ecs::system::ControlSystem::operator()(Registry &reg)
{
    SparseArray<component::Controllable> &controllables = reg.getComponents<component::Controllable>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();

    for (std::size_t i = 0; i < controllables.size() && i < velocities.size(); ++i) {
        if (controllables[i] && velocities[i]) {
            if (!controllables[i]->isControllable)
                continue;
            if (IsKeyDown(KEY_UP)) {
                velocities[i]->vx = 0;
                velocities[i]->vy = NEGATIVE_VELOCITY;
            } else if (IsKeyDown(KEY_DOWN)) {
                velocities[i]->vx = 0;
                velocities[i]->vy = POSITIVE_VELOCITY;
            } else if (IsKeyDown(KEY_LEFT)) {
                velocities[i]->vx = NEGATIVE_VELOCITY;
                velocities[i]->vy = 0;
            } else if (IsKeyDown(KEY_RIGHT)) {
                velocities[i]->vx = POSITIVE_VELOCITY;
                velocities[i]->vy = 0;
            } else {
                velocities[i]->vx = 0;
                velocities[i]->vy = 0;
            }
        }
    }
}
