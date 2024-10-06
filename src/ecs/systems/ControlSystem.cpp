/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** ControlSystem
*/

#include "ControlSystem.hpp"

void ecs::system::ControlSystem::operator()(Registry &reg) {
    SparseArray<component::Controllable> &controllables = reg.getComponents<component::Controllable>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    std::map<std::size_t, entity_t> player = reg.getPlayers();
    int gamepad = 0;

    if (player.size() != 1)
        return;
    entity_t playerEntity = player[0];
    if (controllables[playerEntity] && velocities[playerEntity]) {
        if (!controllables[playerEntity]->isControllable)
            return;
        // velocities[playerEntity]->vx = 0;
        // velocities[playerEntity]->vy = 0;
        if (IsKeyDown(KEY_UP)) {
            velocities[playerEntity]->vx = 0;
            velocities[playerEntity]->vy = -velocities[playerEntity]->possibleVelocity;
        } else if (IsKeyDown(KEY_DOWN)) {
            velocities[playerEntity]->vx = 0;
            velocities[playerEntity]->vy = velocities[playerEntity]->possibleVelocity;
        } else if (IsKeyDown(KEY_LEFT)) {
            velocities[playerEntity]->vy = 0;
            velocities[playerEntity]->vx = -velocities[playerEntity]->possibleVelocity;
        } else if (IsKeyDown(KEY_RIGHT)) {
            velocities[playerEntity]->vy = 0;
            velocities[playerEntity]->vx = velocities[playerEntity]->possibleVelocity;
        } else if (IsGamepadAvailable(gamepad)) {
            handleController(gamepad, velocities, playerEntity);
        }
    }
}

void ecs::system::ControlSystem::handleController(int &gamepad, SparseArray<component::Velocity> &velocities,
                                                  const std::size_t &playerIndex) {
    float leftJoystickX = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_X);
    float leftJoystickY = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_Y);

    if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_UP) || leftJoystickY < -JOYSTICK_SENSIBILITY)
        velocities[playerIndex]->vy = -velocities[playerIndex]->possibleVelocity;
    if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_DOWN) || leftJoystickY > JOYSTICK_SENSIBILITY)
        velocities[playerIndex]->vy = velocities[playerIndex]->possibleVelocity;
    if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_LEFT) || leftJoystickX < -JOYSTICK_SENSIBILITY)
        velocities[playerIndex]->vx = -velocities[playerIndex]->possibleVelocity;
    if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_RIGHT) || leftJoystickX > JOYSTICK_SENSIBILITY)
        velocities[playerIndex]->vx = velocities[playerIndex]->possibleVelocity;
}
