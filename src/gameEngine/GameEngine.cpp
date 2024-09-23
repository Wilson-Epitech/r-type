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
        EndDrawing();
        DrawFPS(SCREEN_WIDTH - FPS_MARGIN, MARGIN_TOP);
        _reg.runSystems();
        if (_gameState(_reg) != game::GameStatus::PLAYING)
            CloseWindow();
    }
}
