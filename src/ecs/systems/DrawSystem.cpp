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
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    float heightScaleFactor = (float)SCREEN_HEIGHT / (float)GetScreenHeight();
    float widthScaleFactor = (float)SCREEN_WIDTH / (float)GetScreenWidth();

    loadTextures(drawables, spriteInfos);
    for (std::size_t i = 0; i < drawables.size() && i < positions.size() && i < spriteInfos.size(); ++i) {
        if (drawables[i] && positions[i] && spriteInfos[i]) {
            if (drawables[i]->isLoaded && !drawables[i]->isBackground) {
                Rectangle destRec = {positions[i]->x / widthScaleFactor, positions[i]->y / heightScaleFactor,
                    drawables[i]->spriteRect.width * (spriteInfos[i]->scaleFactor / widthScaleFactor),
                    drawables[i]->spriteRect.height * (spriteInfos[i]->scaleFactor / heightScaleFactor)};
                DrawTexturePro(drawables[i]->spriteTexture, drawables[i]->spriteRect, destRec, {0, 0}, 0, RAYWHITE);
            } else if (drawables[i]->isLoaded && drawables[i]->isBackground) {
                drawBackground(*velocities[i], *drawables[i], *positions[i], *spriteInfos[i]);
            }
        }
    }
}

void ecs::system::DrawSystem::loadTextures(SparseArray<component::Drawable> &drawables, SparseArray<component::SpriteInfos> &spriteInfos)
{
    float offset = 0.0;
    float firstPatternLocation = 0.0;

    for (std::size_t i = 0; i < drawables.size() && i < spriteInfos.size(); ++i) {
        if (!drawables[i])
            continue;
        if (drawables[i]->isLoaded == false && spriteInfos[i]->spritePath.length() > 0) {
            if (!FileExists(spriteInfos[i]->spritePath.c_str()))
                throw Error("Invalid sprite path");
            drawables[i]->spriteTexture = LoadTexture(spriteInfos[i]->spritePath.c_str());
            offset = float(drawables[i]->spriteTexture.width)/spriteInfos[i]->patternNb;
            firstPatternLocation = spriteInfos[i]->firstPattern * offset;
            drawables[i]->spriteRect = {firstPatternLocation, 0, offset, float(drawables[i]->spriteTexture.height)};
            drawables[i]->isLoaded = true;
        }
    }
}

void ecs::system::DrawSystem::drawBackground(component::Velocity &velocities, component::Drawable &drawables, component::Position &positions, component::SpriteInfos &spriteInfos)
{
    Vector2 backgroundNewPos = {positions.x, positions.y};
    float spriteWidth = drawables.spriteTexture.width * spriteInfos.scaleFactor;
    float spriteHeight = drawables.spriteTexture.height * spriteInfos.scaleFactor;
    int posX = 0;
    int posY = 0;
    int nbrRepeatX = getNbrRepeatBackground(spriteWidth, SCREEN_WIDTH);
    float heightScaleFactor = (float)SCREEN_HEIGHT / (float)GetScreenHeight();
    float widthScaleFactor = (float)SCREEN_WIDTH / (float)GetScreenWidth();

    changeBackgroundPos(velocities, drawables, positions, spriteInfos);
    for (int i = 0; i < getNbrRepeatBackground(spriteHeight, SCREEN_HEIGHT); i++) {
        for (int k = 0; k < nbrRepeatX; k++) {
            backgroundNewPos = {positions.x, positions.y};
            for (int j = 0; j < nbrRepeatX; j++) {
                Rectangle destRec = {(backgroundNewPos.x + posX) / widthScaleFactor, (backgroundNewPos.y + posY) / heightScaleFactor,
                    drawables.spriteRect.width * (spriteInfos.scaleFactor / widthScaleFactor),
                    drawables.spriteRect.height * (spriteInfos.scaleFactor / heightScaleFactor)};
                DrawTexturePro(drawables.spriteTexture, drawables.spriteRect, destRec, {0, 0}, 0, RAYWHITE);
                backgroundNewPos = changeNewBackgroundPos(velocities, drawables, positions, spriteInfos, j);
            }
            posX += spriteWidth;
        }
        posX = 0;
        posY += spriteHeight;
    }
}

int ecs::system::DrawSystem::getNbrRepeatBackground(float spriteSize, int screenSize)
{
    int result = 0;
    float totalSize = 0;

    while (totalSize < screenSize) {
        totalSize += spriteSize;
        result += 1;
    }
    return result + 1;
}

Vector2 ecs::system::DrawSystem::changeNewBackgroundPos(component::Velocity &velocities, component::Drawable &drawables, component::Position &positions, component::SpriteInfos &spriteInfos, int i)
{
    float spriteWidth = drawables.spriteTexture.width * spriteInfos.scaleFactor;
    float spriteHeight = drawables.spriteTexture.height * spriteInfos.scaleFactor;
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

void ecs::system::DrawSystem::changeBackgroundPos(component::Velocity &velocities, component::Drawable &drawables, component::Position &positions, component::SpriteInfos &spriteInfos)
{
    float spriteWidth = drawables.spriteTexture.width * spriteInfos.scaleFactor;
    float spriteHeight = drawables.spriteTexture.height * spriteInfos.scaleFactor;

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
