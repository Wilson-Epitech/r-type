/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** GameEngine
*/

#include "GameEngine.hpp"

GameEngine::GameEngine(ecs::Registry &reg, std::string &gameName) : _reg(reg)
{
    SetTraceLogLevel(TraceLogLevel::LOG_FATAL);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, gameName.c_str());
    MaximizeWindow();
}

void GameEngine::run()
{
    SetTargetFPS(FPS_LIMIT);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
        DrawFPS(SCREEN_WIDTH - FPS_MARGIN, MARGIN_TOP);
        _reg.runSystems();
    }
}
