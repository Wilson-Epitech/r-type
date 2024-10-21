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
            if (drawables[i]->isLoaded && types[i]->entityType != ecs::EntityType::BACKGROUND) {
                Rectangle destRec = {positions[i]->x / screenRatio.x, positions[i]->y / screenRatio.y,
                                     drawables[i]->spriteRect.width * (spriteInfos[i]->scaleFactor / screenRatio.x),
                                     drawables[i]->spriteRect.height * (spriteInfos[i]->scaleFactor / screenRatio.y)};
                DrawTexturePro(drawables[i]->spriteTexture, drawables[i]->spriteRect, destRec, {0, 0}, 0, spriteInfos[i]->color);
                drawLife(reg, drawables, spriteInfos, positions, i);
            } else if (drawables[i]->isLoaded && types[i]->entityType == ecs::EntityType::BACKGROUND) {
                drawBackground(velocities, drawables, positions, spriteInfos, i);
            }
        }
    }
    drawText(reg);
}

void ecs::system::DrawSystem::drawText(Registry &reg) {
    SparseArray<component::TextInfos> &textInfos = reg.getComponents<component::TextInfos>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    std::map<std::size_t, entity_t> player = reg.getPlayers();
    Vector2 screenRatio = Lib::getScreenRatio();
    if (player.size() != 1)
        return;
    entity_t playerEntity = player[0];

    loadFonts(textInfos);
    for (std::size_t i = 0; i < textInfos.size() && i < types.size(); ++i) {
        if (textInfos[i] && types[i]) {
            if (types[i]->entityType == ecs::EntityType::PLAYER && i != playerEntity)
                continue;
            if (textInfos[i]->textToDisplay.length() > 0) {
                DrawTextEx(textInfos[i]->font, textInfos[i]->textToDisplay.c_str(), {(float)textInfos[i]->x / screenRatio.x, (float)textInfos[i]->y / screenRatio.y},
                           textInfos[i]->size / screenRatio.x, LETTER_SPACING, textInfos[i]->color);
            }
        }
    }
}

void ecs::system::DrawSystem::loadFonts(SparseArray<component::TextInfos> &textInfos) {
    for (std::size_t i = 0; i < textInfos.size(); ++i) {
        if (!textInfos[i])
            continue;
        if (!textInfos[i]->fontIsLoaded && textInfos[i]->fontPath.length() > 0) {
            if (!FileExists(textInfos[i]->fontPath.c_str())) {
                textInfos[i]->font = GetFontDefault();
                textInfos[i]->fontIsLoaded = true;
            }
            textInfos[i]->font = LoadFont(textInfos[i]->fontPath.c_str());
            textInfos[i]->fontIsLoaded = true;
        }
    }
}

void ecs::system::DrawSystem::drawLife(Registry &reg, SparseArray<component::Drawable> &drawables,
                                       SparseArray<component::SpriteInfos> &spriteInfos, SparseArray<component::Position> &positions,
                                       std::size_t index) {
    SparseArray<component::Life> &lifes = reg.getComponents<component::Life>();
    Vector2 screenRatio = Lib::getScreenRatio();
    float lifeBarSize = 0;
    CLITERAL(Color) lifeColor = GREEN;

    if (lifes.size() < index)
        return;
    if (!lifes[index])
        return;
    if (!lifes[index]->displayLife)
        return;

    lifeBarSize = ((drawables[index]->spriteRect.width * (float)lifes[index]->nbLives / lifes[index]->maxLives) *
                   (spriteInfos[index]->scaleFactor / screenRatio.x)) *
                  2 / 3;
    if ((float)lifes[index]->nbLives / (float)lifes[index]->maxLives <= (float)1 / 3)
        lifeColor = RED;
    else if ((float)lifes[index]->nbLives / (float)lifes[index]->maxLives <= (float)2 / 3)
        lifeColor = ORANGE;
    else if (lifes[index]->isInvincible)
        lifeColor = PINK;
    DrawRectangle((positions[index]->x + drawables[index]->spriteRect.width / 2) / screenRatio.x, positions[index]->y / screenRatio.y,
                  lifeBarSize, LIVES_BAR_HEIGHT / screenRatio.y, lifeColor);
    if (LIVES_BAR_HEIGHT / screenRatio.y > 3)
        DrawRectangleLines((positions[index]->x + drawables[index]->spriteRect.width / 2) / screenRatio.x,
                           positions[index]->y / screenRatio.y,
                           (drawables[index]->spriteRect.width * (spriteInfos[index]->scaleFactor / screenRatio.x) + 1) * 2 / 3,
                           (LIVES_BAR_HEIGHT + 1) / screenRatio.y, lifeColor);
}

void ecs::system::DrawSystem::loadTextures(SparseArray<component::Drawable> &drawables, SparseArray<component::SpriteInfos> &spriteInfos) {
    float offset = 0.0;
    float firstPatternLocation = 0.0;

    for (std::size_t i = 0; i < drawables.size() && i < spriteInfos.size(); ++i) {
        if (!drawables[i] || !spriteInfos[i])
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
