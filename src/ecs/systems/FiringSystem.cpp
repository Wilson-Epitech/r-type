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
                addShot(i, reg);
            }
        }
    }
}

void ecs::system::FiringSystem::addShot(std::size_t playerIndex, Registry &reg)
{
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    SparseArray<component::Drawable> &drawables = reg.getComponents<component::Drawable>();
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    entity_t entity = reg.spawnEntity();
    Texture2D shotTexture = LoadTexture(PLAYERSHOT_PATH);
    float offset = float(shotTexture.width)/PLAYERSHOT_SPRITE_NB_PATTERN;
    Rectangle shotSpriteRect = {0, 0, offset, float(shotTexture.height)};
    float shotX = positions[playerIndex]->x + float(drawables[playerIndex]->spriteRect.width) + float(shotSpriteRect.width);
    float shotY = positions[playerIndex]->y + (float(drawables[playerIndex]->spriteRect.height)/2 -
        float(drawables[playerIndex]->spriteRect.height)/3) + (float(shotSpriteRect.height)/2);

    positions.insertAt(entity, ecs::component::Position{shotX, shotY});
    velocities.insertAt(entity, ecs::component::Velocity{3.0f, 0});
    drawables.insertAt(entity, ecs::component::Drawable{PLAYERSHOT_PATH, PLAYERSHOT_SPRITE_NB_PATTERN});
    types.insertAt(entity, ecs::component::Type{ecs::EntityType::PLAYERSHOT});
    lives.insertAt(entity, ecs::component::Life{1});
}
