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
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();

    loadTextures(drawables);
    for (std::size_t i = 0; i < drawables.size() && i < positions.size(); ++i) {
        if (drawables[i] && positions[i]) {
            if (drawables[i]->isLoaded && !drawables[i]->isBackground) {
                Rectangle destRec = {positions[i]->x, positions[i]->y,
                    drawables[i]->spriteRect.width * drawables[i]->scaleFactor,
                    drawables[i]->spriteRect.height * drawables[i]->scaleFactor};
                DrawTexturePro(drawables[i]->spriteTexture, drawables[i]->spriteRect, destRec, {0, 0}, 0, RAYWHITE);
            } else if (drawables[i]->isLoaded && drawables[i]->isBackground) {
                drawBackground(*velocities[i], *drawables[i], *positions[i]);
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

void ecs::system::DrawSystem::drawBackground(component::Velocity &velocities, component::Drawable &drawables, component::Position &positions)
{
    Vector2 backgroundNewPos = {positions.x, positions.y};
    float spriteWidth = drawables.spriteTexture.width * drawables.scaleFactor;
    float spriteHeight = drawables.spriteTexture.height * drawables.scaleFactor;
    int posX = 0;
    int posY = 0;
    int nbrRepeatX = getNbrRepeatBackground(drawables, spriteWidth, SCREEN_WIDTH);

    changeBackgroundPos(velocities, drawables, positions);
    for (int i = 0; i < getNbrRepeatBackground(drawables, spriteHeight, SCREEN_HEIGHT); i++) {
        for (int k = 0; k < nbrRepeatX; k++) {
            backgroundNewPos = {positions.x, positions.y};
            for (int j = 0; j < nbrRepeatX; j++) {
                Rectangle destRec = {backgroundNewPos.x + posX, backgroundNewPos.y + posY,
                    drawables.spriteRect.width * drawables.scaleFactor,
                    drawables.spriteRect.height * drawables.scaleFactor};
                DrawTexturePro(drawables.spriteTexture, drawables.spriteRect, destRec, {0, 0}, 0, RAYWHITE);
                backgroundNewPos = changeNewBackgroundPos(velocities, drawables, positions, j);
            }
            posX += spriteWidth;
        }
        posX = 0;
        posY += spriteHeight;
    }
}

int ecs::system::DrawSystem::getNbrRepeatBackground(component::Drawable &drawables, float spriteSize, int screenSize)
{
    int result = 0;
    float totalSize = 0;

    while (totalSize < screenSize) {
        totalSize += spriteSize;
        result += 1;
    }
    return result + 1;
}

Vector2 ecs::system::DrawSystem::changeNewBackgroundPos(component::Velocity &velocities, component::Drawable &drawables, component::Position &positions, int i)
{
    float spriteWidth = drawables.spriteTexture.width * drawables.scaleFactor;
    float spriteHeight = drawables.spriteTexture.height * drawables.scaleFactor;
    Vector2 backgroundNewPos = {0, 0};

    if (velocities.vx > 0.000) {
        backgroundNewPos.x = positions.x - spriteWidth * (i + 1);
    } else if (velocities.vx < 0.000) {
        backgroundNewPos.x = positions.x + spriteWidth * (i + 1);
    }
    if (velocities.vy > 0.000) {
        backgroundNewPos.y = positions.y - spriteHeight * (i + 1);
    } else if (velocities.vy < 0.000) {
        backgroundNewPos.y = positions.y + spriteHeight * (i + 1);
    }

    return backgroundNewPos;
}

void ecs::system::DrawSystem::changeBackgroundPos(component::Velocity &velocities, component::Drawable &drawables, component::Position &positions)
{
    float spriteWidth = drawables.spriteTexture.width * drawables.scaleFactor;
    float spriteHeight = drawables.spriteTexture.height * drawables.scaleFactor;

    if (velocities.vx < 0.000 && positions.x + spriteWidth <= 1) {
        positions.x = positions.x + spriteWidth;
    }
    if (velocities.vx > 0.000 && positions.x >= SCREEN_WIDTH) {
        positions.x = SCREEN_WIDTH - spriteWidth;
    }
    if (velocities.vy < 0.000 && positions.y + spriteHeight <= 1) {
        positions.y = positions.y + spriteHeight;
    }
    if (velocities.vy > 0.000 && positions.y >= SCREEN_HEIGHT) {
        positions.y = SCREEN_HEIGHT - spriteHeight;
    }
}
