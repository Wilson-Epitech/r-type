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
            if (drawables[i]->isLoaded) {
                Rectangle destRec = {positions[i]->x, positions[i]->y,
                    drawables[i]->spriteRect.width * drawables[i]->scaleFactor,
                    drawables[i]->spriteRect.height * drawables[i]->scaleFactor};
                DrawTexturePro(drawables[i]->spriteTexture, drawables[i]->spriteRect, destRec, {0, 0}, 0, RAYWHITE);
            }
        }
    }
}

void ecs::system::DrawSystem::loadTextures(SparseArray<component::Drawable> &drawables)
{
    float offset = 0.0;
    float firstPatternLocation = 0.0;

    for (std::size_t i = 0; i < drawables.size(); ++i) {
        if (!drawables[i])
            continue;
        if (drawables[i]->isLoaded == false && drawables[i]->spritePath.length() > 0) {
            drawables[i]->spriteTexture = LoadTexture(drawables[i]->spritePath.c_str());
            offset = float(drawables[i]->spriteTexture.width)/drawables[i]->patternNb;
            firstPatternLocation = drawables[i]->firstPattern * offset;
            drawables[i]->spriteRect = {firstPatternLocation, 0, offset, float(drawables[i]->spriteTexture.height)};
            drawables[i]->isLoaded = true;
        }
    }
}
