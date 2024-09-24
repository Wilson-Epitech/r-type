/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** GameEngine
*/

#include "GameEngine.hpp"

void GameEngine::run()
{
    SetTargetFPS(FPS_LIMIT);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        _reg.runSystems();
        DrawFPS(50, 50);
        EndDrawing();
        if (_gameState(_reg) != game::GameStatus::PLAYING)
            CloseWindow();
    }
}
