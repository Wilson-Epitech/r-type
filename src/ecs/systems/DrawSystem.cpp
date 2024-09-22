/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** DrawSystem
*/

#include "DrawSystem.hpp"

void ecs::system::DrawSystem::operator()(Registry &reg)
{
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Drawable> &drawables = reg.getComponents<component::Drawable>();
    loadTextures(drawables);
    for (std::size_t i = 0; i < drawables.size() && i < positions.size(); ++i) {
        if (drawables[i] && positions[i]) {
            if (drawables[i]->spritePath.length() > 0) {
                DrawTexture(drawables[i]->spriteTexture, positions[i]->x, positions[i]->y, RAYWHITE);
            }
        }
    }
}

void ecs::system::DrawSystem::loadTextures(SparseArray<component::Drawable> &drawables)
{
    for (std::size_t i = 0; i < drawables.size(); ++i) {
        if (drawables[i]->isLoaded == false) {
            if (drawables[i]->spritePath.length() > 0) {
                drawables[i]->spriteTexture = LoadTexture(drawables[i]->spritePath.c_str());
            }
            drawables[i]->isLoaded = true;
        }
    }
}
