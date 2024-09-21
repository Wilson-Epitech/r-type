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

int main()
{
    game::rtypeGame rtype;

    auto &key = rtype.getKey();
    auto &reg = rtype.getRegister();

    SetTraceLogLevel(TraceLogLevel::LOG_FATAL);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "R-Type");
    MaximizeWindow();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        if (IsKeyPressed(KEY_UP))
            *key = KeyPressed::UP;
        if (IsKeyPressed(KEY_DOWN))
            *key = KeyPressed::DOWN;
        if (IsKeyPressed(KEY_LEFT))
            *key = KeyPressed::LEFT;
        if (IsKeyPressed(KEY_RIGHT))
            *key = KeyPressed::RIGHT;
        reg.runSystems();
        DrawFPS(SCREEN_WIDTH - FPS_MARGIN, MARGIN_TOP);
        EndDrawing();
    }
    return 0;
}
