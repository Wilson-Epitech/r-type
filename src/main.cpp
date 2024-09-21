/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** main
*/

/**
 * @brief this is the main function,
 * ...
 * @return int
 */

#include "rtypeGame.hpp"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define MARGIN_LEFT 10
#define MARGIN_TOP 10
#define FPS_MARGIN 100
#define WAIT_DURATION 0.016

int main()
{
    game::rtypeGame rtype;

    auto &key = rtype.getKey();
    auto &reg = rtype.getRegister();

    double previousTimer = GetTime();
    double timeElapse = 0.0;
    double waitTime = 0.0;

    SetTraceLogLevel(TraceLogLevel::LOG_FATAL);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "R-Type");
    MaximizeWindow();
    // SetTargetFPS(60);

    while (!WindowShouldClose()) {
        previousTimer = GetTime();
        waitTime = previousTimer - timeElapse;

        BeginDrawing();
        if (waitTime >= WAIT_DURATION) {
            ClearBackground(RAYWHITE);
            timeElapse = previousTimer;
            *key = KeyPressed::NO_KEY;
            if (IsKeyDown(KEY_UP))
                *key = KeyPressed::UP;
            if (IsKeyDown(KEY_DOWN))
                *key = KeyPressed::DOWN;
            if (IsKeyDown(KEY_LEFT))
                *key = KeyPressed::LEFT;
            if (IsKeyDown(KEY_RIGHT))
                *key = KeyPressed::RIGHT;
            reg.runSystems();
            reg.runDraw();
        } else {
            ClearBackground(RAYWHITE);
            reg.runDraw();
        }
        EndDrawing();
        DrawFPS(SCREEN_WIDTH - FPS_MARGIN, MARGIN_TOP);
    }
    return 0;
}
