/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** lib
*/

#include "Lib.hpp"

int Lib::getScreenHeight() {
    return SCREEN_HEIGHT;
}

int Lib::getScreenWidth() {
    return SCREEN_WIDTH;
}

bool Lib::isOnScreen(ecs::CustomVector2D pos) {
    if (pos.x > Lib::getScreenWidth() || pos.x < 0)
        return false;
    if (pos.y > Lib::getScreenHeight() || pos.y < 0)
        return false;
    return true;
}

double Lib::toMilliseconds(double time) {
    return time * TO_MILLISECONDS;
}
