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
    int gamepad = 0;

    for (std::size_t i = 0; i < controllables.size() && i < velocities.size(); ++i) {
        if (controllables[i] && velocities[i]) {
            if (!controllables[i]->isControllable)
                continue;
            velocities[i]->vx = 0;
            velocities[i]->vy = 0;
            if (IsKeyDown(KEY_UP))
                velocities[i]->vy = -velocities[i]->possibleVelocity;
            if (IsKeyDown(KEY_DOWN))
                velocities[i]->vy = velocities[i]->possibleVelocity;
            if (IsKeyDown(KEY_LEFT))
                velocities[i]->vx = -velocities[i]->possibleVelocity;
            if (IsKeyDown(KEY_RIGHT))
                velocities[i]->vx = velocities[i]->possibleVelocity;
            else if (IsGamepadAvailable(gamepad)) {
                handleController(gamepad, *velocities[i]);
            }
        }
    }
}

void ecs::system::ControlSystem::handleController(int &gamepad, component::Velocity &velocity)
{
    float leftJoystickX = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_X);
    float leftJoystickY = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_Y);

    if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_UP) || leftJoystickY < -JOYSTICK_SENSIBILITY)
        velocity.vy = -velocity.possibleVelocity;
    if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_DOWN) || leftJoystickY > JOYSTICK_SENSIBILITY)
        velocity.vy = velocity.possibleVelocity;
    if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_LEFT) || leftJoystickX < -JOYSTICK_SENSIBILITY)
        velocity.vx = -velocity.possibleVelocity;
    if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_RIGHT) || leftJoystickX > JOYSTICK_SENSIBILITY)
        velocity.vx = velocity.possibleVelocity;
}
