/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** DrawLifeBarSystem
*/

#include "DrawLifeBarSystem.hpp"

void ecs::system::DrawLifeBarSystem::operator()(Registry &reg) {
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Drawable> &drawables = reg.getComponents<component::Drawable>();
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();

    for (std::size_t i = 0; i < drawables.size() && i < positions.size() && i < spriteInfos.size() && i < lives.size(); ++i) {
        if (drawables[i] && positions[i] && spriteInfos[i] && lives[i]) {
            if (drawables[i]->isLoaded && lives[i]->displayLife) {
                drawLife(drawables, spriteInfos, positions, lives, i);
            }
        }
    }
}

void ecs::system::DrawLifeBarSystem::drawLife(SparseArray<component::Drawable> &drawables, SparseArray<component::SpriteInfos> &spriteInfos,
                                              SparseArray<component::Position> &positions, SparseArray<component::Life> &lives,
                                              const std::size_t &index) {
    ecs::CustomVector2D screenRatio = {(float)Lib::getScreenWidth() / (float)GetScreenWidth(),
                                       (float)Lib::getScreenHeight() / (float)GetScreenHeight()};
    float lifeBarSize = 0;
    CLITERAL(Color) lifeColor = GREEN;

    lifeBarSize = ((drawables[index]->spriteRect.width * (float)lives[index]->nbLives / lives[index]->maxLives) *
                   (spriteInfos[index]->scaleFactor / screenRatio.x)) *
                  2 / 3;
    if ((float)lives[index]->nbLives / (float)lives[index]->maxLives <= (float)1 / 3)
        lifeColor = RED;
    else if ((float)lives[index]->nbLives / (float)lives[index]->maxLives <= (float)2 / 3)
        lifeColor = ORANGE;
    DrawRectangle((positions[index]->x + drawables[index]->spriteRect.width / 2) / screenRatio.x, positions[index]->y / screenRatio.y,
                  lifeBarSize, LIVES_BAR_HEIGHT / screenRatio.y, lifeColor);
    if (LIVES_BAR_HEIGHT / screenRatio.y > 3)
        DrawRectangleLines((positions[index]->x + drawables[index]->spriteRect.width / 2) / screenRatio.x,
                           positions[index]->y / screenRatio.y,
                           (drawables[index]->spriteRect.width * (spriteInfos[index]->scaleFactor / screenRatio.x) + 1) * 2 / 3,
                           (LIVES_BAR_HEIGHT + 1) / screenRatio.y, lifeColor);
}
