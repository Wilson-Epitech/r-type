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
    float spawnInterval = 0.3;

    for (std::size_t i = 0; i < controllables.size() && i < positions.size(); ++i) {
        if (controllables[i] && positions[i]) {
            if (!controllables[i]->isControllable)
                continue;
            if (!IsKeyDown(KEY_SPACE)) {
                canShoot = true;
                time0 = GetTime();
            }
            if (IsKeyDown(KEY_SPACE) && canShoot) {
                addShot(i, reg);
                canShoot = false;
            }
        }
    }
    if (timeElapsed >= spawnInterval) {
        canShoot = true;
        time0 = GetTime();
    }
}

void ecs::system::FiringSystem::addShot(std::size_t playerIndex, Registry &reg)
{
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    SparseArray<component::Drawable> &drawables = reg.getComponents<component::Drawable>();
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    std::pair<ecs::component::PlayerShoot, ecs::component::Velocity> playerShootAndVelocities = getPlayerShoot(reg);
    ecs::component::PlayerShoot playerShoot = playerShootAndVelocities.first;
    ecs::component::Velocity playerShootVelocities = playerShootAndVelocities.second;
    entity_t entity = reg.spawnEntity();
    Texture2D shotTexture = LoadTexture(playerShoot.spritePath.c_str());
    float offset = float(shotTexture.width)/playerShoot.patternNb;
    Rectangle shotSpriteRect = {0, 0, offset, float(shotTexture.height)};
    float shotX = positions[playerIndex]->x + float(drawables[playerIndex]->spriteRect.width) + float(shotSpriteRect.width);
    float shotY = positions[playerIndex]->y + (float(drawables[playerIndex]->spriteRect.height)/2 -
        float(drawables[playerIndex]->spriteRect.height)/3) + (float(shotSpriteRect.height)/2);

    positions.insertAt(entity, ecs::component::Position{shotX, shotY});
    velocities.insertAt(entity, ecs::component::Velocity(playerShootVelocities));
    drawables.insertAt(entity, ecs::component::Drawable{playerShoot.spritePath.c_str(), playerShoot.patternNb, playerShoot.firstPattern, playerShoot.scaleFactor});
    types.insertAt(entity, ecs::component::Type{ecs::EntityType::PLAYERSHOT});
    lives.insertAt(entity, ecs::component::Life{1});
}

std::pair<ecs::component::PlayerShoot, ecs::component::Velocity> ecs::system::FiringSystem::getPlayerShoot(Registry &reg)
{
    SparseArray<component::PlayerShoot> &playerShoot = reg.getComponents<component::PlayerShoot>();
    SparseArray<component::Velocity> &playerShootVelocities = reg.getComponents<component::Velocity>();

    for (std::size_t i = 0; i < playerShoot.size() && i < playerShootVelocities.size(); ++i) {
        if (playerShoot[i] && playerShootVelocities[i]) {
            if (playerShoot[i]->spritePath.length() > 0) {
                return {*playerShoot[i], *playerShootVelocities[i]};
            }
        }
    }
    throw Error("Component PlayerShoot not found have you initialized it ?");
}
