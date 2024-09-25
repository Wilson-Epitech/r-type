/*
** R_TYPE PROJECT, 2024
** responcive_window
** File description:
** lib
*/

#include "Lib.hpp"

Vector2 Lib::getScreenRatio() {
    float widthScaleFactor = (float)SCREEN_WIDTH / (float)GetScreenWidth();
    float heightScaleFactor = (float)SCREEN_HEIGHT / (float)GetScreenHeight();

    return {widthScaleFactor, heightScaleFactor};
}

int Lib::getScreenHeight() {
    return SCREEN_HEIGHT;
}

int Lib::getScreenWidth() {
    return SCREEN_WIDTH;
}
