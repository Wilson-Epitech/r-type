/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** FiringSystem
*/

#include "FiringSystem.hpp"

void ecs::system::FiringSystem::operator()(Registry &reg) {
    SparseArray<component::Controllable> &controllables = reg.getComponents<component::Controllable>();
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    static std::chrono::_V2::steady_clock::time_point time0 = std::chrono::steady_clock::now();
    static bool canShoot = true;
    std::chrono::_V2::steady_clock::time_point time1 = std::chrono::steady_clock::now();
    std::chrono::milliseconds timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(time1 - time0);
    float spawnInterval = SHOOT_RATE;
    int gamepad = 0;

    for (std::size_t i = 0; i < controllables.size() && i < positions.size(); ++i) {
        if (controllables[i] && positions[i]) {
            if (!controllables[i]->isControllable)
                continue;
            if (!IsKeyDown(KEY_SPACE) && !IsGamepadTriggerButtonDown(gamepad)) {
                canShoot = true;
                time0 = std::chrono::steady_clock::now();
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
    if (timeElapsed.count() >= Lib::toMilliseconds(spawnInterval)) {
        canShoot = true;
        time0 = std::chrono::steady_clock::now();
    }
}

bool ecs::system::FiringSystem::IsGamepadTriggerButtonDown(int &gamepad) {
    return (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_TRIGGER_1) || IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_TRIGGER_2) ||
            IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_TRIGGER_1) || IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_TRIGGER_2));
}

void ecs::system::FiringSystem::addShot(std::size_t playerIndex, Registry &reg) {
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    SparseArray<component::Drawable> &drawables = reg.getComponents<component::Drawable>();
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    std::size_t playerShootIndex = getPlayerShootIndex(reg);
    if (playerShootIndex == 0)
        return;
    entity_t entity = reg.spawnEntity();
    Texture2D shotTexture = LoadTexture(spriteInfos[playerShootIndex]->spritePath.c_str());
    float offset = float(shotTexture.width) / spriteInfos[playerShootIndex]->patternNb;
    Rectangle shotSpriteRect = {0, 0, offset, float(shotTexture.height)};
    float shotX = positions[playerIndex]->x + (float(drawables[playerIndex]->spriteRect.width) + float(shotSpriteRect.width)) *
                                                  spriteInfos[playerShootIndex]->scaleFactor;
    float shotY =
        positions[playerIndex]->y + ((float(drawables[playerIndex]->spriteRect.height) / 2) * spriteInfos[playerIndex]->scaleFactor -
                                     (float(shotSpriteRect.height) / 2) * spriteInfos[playerShootIndex]->scaleFactor);

    UnloadTexture(shotTexture);
    soundHasToBePlayed(reg);
    positions.insertAt(entity, ecs::component::Position{shotX, shotY});
    velocities.insertAt(entity, ecs::component::Velocity{velocities[playerShootIndex]->vx, velocities[playerShootIndex]->vy,
                                                         velocities[playerShootIndex]->possibleVelocity});
    spriteInfos.insertAt(
        entity, ecs::component::SpriteInfos{spriteInfos[playerShootIndex]->spritePath, spriteInfos[playerShootIndex]->patternNb,
                                            spriteInfos[playerShootIndex]->firstPattern, spriteInfos[playerShootIndex]->scaleFactor});
    drawables.insertAt(entity, ecs::component::Drawable{});
    types.insertAt(entity, ecs::component::Type{ecs::EntityType::PLAYERSHOT});
    lives.insertAt(entity, ecs::component::Life{1});
}

std::size_t ecs::system::FiringSystem::getPlayerShootIndex(Registry &reg) {
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    SparseArray<component::Velocity> &playerShootVelocities = reg.getComponents<component::Velocity>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();

    for (std::size_t i = 0; i < spriteInfos.size() && i < playerShootVelocities.size() && i < types.size(); ++i) {
        if (spriteInfos[i] && playerShootVelocities[i] && types[i]) {
            if (spriteInfos[i]->spritePath.length() > 0 && types[i]->entityType == ecs::EntityType::PLAYERSHOT_TEMPLATE) {
                return i;
            }
        }
    }
    return 0;
}

void ecs::system::FiringSystem::soundHasToBePlayed(Registry &reg) {
    SparseArray<component::SoundEffect> &soundEffect = reg.getComponents<component::SoundEffect>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();

    for (std::size_t i = 0; i < types.size() && i < soundEffect.size(); ++i) {
        if (types[i] && soundEffect[i]) {
            if (types[i]->entityType == ecs::EntityType::PLAYERSHOT) {
                soundEffect[i]->hasToBePlayed = true;
            }
        }
    }
}
