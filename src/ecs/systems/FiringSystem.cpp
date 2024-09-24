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
    static double time0 = GetTime();
    static bool canShoot = true;
    double time1 = GetTime();
    double timeElapsed = time1 - time0;
    float spawnInterval = SHOOT_RATE;
    int gamepad = 0;

    for (std::size_t i = 0; i < controllables.size() && i < positions.size(); ++i) {
        if (controllables[i] && positions[i]) {
            if (!controllables[i]->isControllable)
                continue;
            if (!IsKeyDown(KEY_SPACE) && !IsGamepadTriggerButtonDown(gamepad)) {
                canShoot = true;
                time0 = GetTime();
            }
            if (IsKeyDown(KEY_SPACE) && canShoot) {
                addShot(i, reg);
                canShoot = false;
            } else if (IsGamepadAvailable(gamepad)) {
                if (IsGamepadTriggerButtonDown(gamepad) && canShoot) {
                    addShot(i, reg);
                    canShoot = false;
                }
            }
        }
    }
    if (timeElapsed >= spawnInterval) {
        canShoot = true;
        time0 = GetTime();
    }
}

bool ecs::system::FiringSystem::IsGamepadTriggerButtonDown(int &gamepad)
{
    return (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_TRIGGER_1) ||
    IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_TRIGGER_2) ||
    IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_TRIGGER_1) ||
    IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_TRIGGER_2));
}

void ecs::system::FiringSystem::addShot(std::size_t playerIndex, Registry &reg)
{
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    SparseArray<component::Drawable> &drawables = reg.getComponents<component::Drawable>();
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    std::size_t playerShootIndex = getPlayerShootIndex(reg);
    entity_t entity = reg.spawnEntity();
    Texture2D shotTexture = LoadTexture(spriteInfos[playerShootIndex]->spritePath.c_str());
    float offset = float(shotTexture.width)/spriteInfos[playerShootIndex]->patternNb;
    Rectangle shotSpriteRect = {0, 0, offset, float(shotTexture.height)};
    float shotX = positions[playerIndex]->x + float(drawables[playerIndex]->spriteRect.width) + float(shotSpriteRect.width);
    float shotY = positions[playerIndex]->y + (float(drawables[playerIndex]->spriteRect.height)/2 -
        float(drawables[playerIndex]->spriteRect.height)/3) + (float(shotSpriteRect.height)/2);

    positions.insertAt(entity, ecs::component::Position{shotX, shotY});
    velocities.insertAt(entity, ecs::component::Velocity{velocities[playerShootIndex]->vx,
        velocities[playerShootIndex]->vy, velocities[playerShootIndex]->possibleVelocity});
    spriteInfos.insertAt(entity, ecs::component::SpriteInfos{spriteInfos[playerShootIndex]->spritePath,
        spriteInfos[playerShootIndex]->patternNb, spriteInfos[playerShootIndex]->firstPattern, spriteInfos[playerShootIndex]->scaleFactor});
    drawables.insertAt(entity, ecs::component::Drawable{});
    types.insertAt(entity, ecs::component::Type{ecs::EntityType::PLAYERSHOT});
    lives.insertAt(entity, ecs::component::Life{1});
}

std::size_t ecs::system::FiringSystem::getPlayerShootIndex(Registry &reg)
{
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    SparseArray<component::Velocity> &playerShootVelocities = reg.getComponents<component::Velocity>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();

    for (std::size_t i = 0; i < spriteInfos.size() && i < playerShootVelocities.size() && i < types.size(); ++i) {
        if (spriteInfos[i] && playerShootVelocities[i] && types[i]) {
            if (spriteInfos[i]->spritePath.length() > 0 && types[i]->type == ecs::EntityType::PLAYERSHOT) {
                return i;
            }
        }
    }
    throw Error("Component PlayerShoot not found have you initialized it ?");
}
