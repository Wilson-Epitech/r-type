/*
** R_TYPE PROJECT, 2024
** r-type
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

bool Lib::isOnScreen(Vector2 pos) {
    if (pos.x > Lib::getScreenWidth() || pos.x < 0)
        return false;
    if (pos.y > Lib::getScreenHeight() || pos.y < 0)
        return false;
    return true;
}

double Lib::toMilliseconds(double time) {
    return time * TO_MILLISECONDS;
}

bool Lib::isTimeElapsed(const std::chrono::steady_clock::time_point &time, const float &duration) {
    std::chrono::steady_clock::time_point actualTime = std::chrono::steady_clock::now();
    std::chrono::milliseconds timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(actualTime - time);

    return timeElapsed.count() >= Lib::toMilliseconds(duration);
}
