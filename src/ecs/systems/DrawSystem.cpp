/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** DrawSystem
*/

#include "DrawSystem.hpp"

void ecs::system::DrawSystem::operator()(Registry &reg) {
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Drawable> &drawables = reg.getComponents<component::Drawable>();
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    Vector2 screenRatio = Lib::getScreenRatio();

    loadTextures(drawables, spriteInfos);
    for (std::size_t i = 0; i < drawables.size() && i < positions.size() && i < spriteInfos.size() && i < types.size(); ++i) {
        if (drawables[i] && positions[i] && spriteInfos[i] && velocities[i] && types[i]) {
            if (drawables[i]->isLoaded && types[i]->type != ecs::EntityType::BACKGROUND) {
                Rectangle destRec = {positions[i]->x / screenRatio.x, positions[i]->y / screenRatio.y,
                                     drawables[i]->spriteRect.width * (spriteInfos[i]->scaleFactor / screenRatio.x),
                                     drawables[i]->spriteRect.height * (spriteInfos[i]->scaleFactor / screenRatio.y)};
                DrawTexturePro(drawables[i]->spriteTexture, drawables[i]->spriteRect, destRec, {0, 0}, 0, RAYWHITE);
            } else if (drawables[i]->isLoaded && types[i]->type == ecs::EntityType::BACKGROUND) {
                drawBackground(velocities, drawables, positions, spriteInfos, i);
            }
        }
    }
}

void ecs::system::DrawSystem::loadTextures(SparseArray<component::Drawable> &drawables, SparseArray<component::SpriteInfos> &spriteInfos) {
    float offset = 0.0;
    float firstPatternLocation = 0.0;

    for (std::size_t i = 0; i < drawables.size() && i < spriteInfos.size(); ++i) {
        if (!drawables[i])
            continue;
        if (drawables[i]->isLoaded == false && spriteInfos[i]->spritePath.length() > 0) {
            if (!FileExists(spriteInfos[i]->spritePath.c_str()))
                throw Error("Invalid sprite path");
            drawables[i]->spriteTexture = LoadTexture(spriteInfos[i]->spritePath.c_str());
            offset = float(drawables[i]->spriteTexture.width) / spriteInfos[i]->patternNb;
            firstPatternLocation = spriteInfos[i]->firstPattern * offset;
            drawables[i]->spriteRect = {firstPatternLocation, 0, offset, float(drawables[i]->spriteTexture.height)};
            drawables[i]->isLoaded = true;
        }
    }
}

void ecs::system::DrawSystem::drawBackground(SparseArray<component::Velocity> &velocities, SparseArray<component::Drawable> &drawables,
                                             SparseArray<component::Position> &positions, SparseArray<component::SpriteInfos> &spriteInfos,
                                             const std::size_t &index) {
    Vector2 backgroundNewPos = {positions[index]->x, positions[index]->y};
    float spriteWidth = drawables[index]->spriteTexture.width * spriteInfos[index]->scaleFactor;
    float spriteHeight = drawables[index]->spriteTexture.height * spriteInfos[index]->scaleFactor;
    int posX = 0;
    int posY = 0;
    int nbrRepeatX = getNbrRepeatBackground(spriteWidth, Lib::getScreenWidth());
    Vector2 screenRatio = Lib::getScreenRatio();

    changeBackgroundPos(velocities, drawables, positions, spriteInfos, index);
    for (int i = 0; i < getNbrRepeatBackground(spriteHeight, Lib::getScreenHeight()); i++) {
        for (int k = 0; k < nbrRepeatX; k++) {
            backgroundNewPos = {positions[index]->x, positions[index]->y};
            for (int j = 0; j < nbrRepeatX; j++) {
                Rectangle destRec = {(backgroundNewPos.x + posX) / screenRatio.x, (backgroundNewPos.y + posY) / screenRatio.y,
                                     drawables[index]->spriteRect.width * (spriteInfos[index]->scaleFactor / screenRatio.x),
                                     drawables[index]->spriteRect.height * (spriteInfos[index]->scaleFactor / screenRatio.y)};
                DrawTexturePro(drawables[index]->spriteTexture, drawables[index]->spriteRect, destRec, {0, 0}, 0, RAYWHITE);
                backgroundNewPos = changeNewBackgroundPos(velocities, drawables, positions, spriteInfos, index, j);
            }
            posX += spriteWidth;
        }
        posX = 0;
        posY += spriteHeight;
    }
}

int ecs::system::DrawSystem::getNbrRepeatBackground(float spriteSize, int screenSize) {
    int result = 0;
    float totalSize = 0;

    while (totalSize < screenSize) {
        totalSize += spriteSize;
        result += 1;
    }
    return result + 1;
}

Vector2 ecs::system::DrawSystem::changeNewBackgroundPos(SparseArray<component::Velocity> &velocities,
                                                        SparseArray<component::Drawable> &drawables,
                                                        SparseArray<component::Position> &positions,
                                                        SparseArray<component::SpriteInfos> &spriteInfos, const std::size_t &index, int j) {
    float spriteWidth = drawables[index]->spriteTexture.width * spriteInfos[index]->scaleFactor;
    float spriteHeight = drawables[index]->spriteTexture.height * spriteInfos[index]->scaleFactor;
    Vector2 backgroundNewPos = {0, 0};

    if (velocities[index]->vx > 0.000) {
        backgroundNewPos.x = positions[index]->x - spriteWidth * (j + 1);
    } else if (velocities[index]->vx < 0.000) {
        backgroundNewPos.x = positions[index]->x + spriteWidth * (j + 1);
    }
    if (velocities[index]->vy > 0.000) {
        backgroundNewPos.y = positions[index]->y - spriteHeight * (j + 1);
    } else if (velocities[index]->vy < 0.000) {
        backgroundNewPos.y = positions[index]->y + spriteHeight * (j + 1);
    }

    return backgroundNewPos;
}

void ecs::system::DrawSystem::changeBackgroundPos(SparseArray<component::Velocity> &velocities, SparseArray<component::Drawable> &drawables,
                                                  SparseArray<component::Position> &positions,
                                                  SparseArray<component::SpriteInfos> &spriteInfos, const std::size_t &index) {
    float spriteWidth = drawables[index]->spriteTexture.width * spriteInfos[index]->scaleFactor;
    float spriteHeight = drawables[index]->spriteTexture.height * spriteInfos[index]->scaleFactor;

    if (velocities[index]->vx < 0.000 && positions[index]->x + spriteWidth <= 1) {
        positions[index]->x = positions[index]->x + spriteWidth;
    }
    if (velocities[index]->vx > 0.000 && positions[index]->x >= Lib::getScreenWidth()) {
        positions[index]->x = Lib::getScreenWidth() - spriteWidth;
    }
    if (velocities[index]->vy < 0.000 && positions[index]->y + spriteHeight <= 1) {
        positions[index]->y = positions[index]->y + spriteHeight;
    }
    if (velocities[index]->vy > 0.000 && positions[index]->y >= Lib::getScreenHeight()) {
        positions[index]->y = Lib::getScreenHeight() - spriteHeight;
    }
}
