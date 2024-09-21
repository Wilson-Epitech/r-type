/*
** EPATECH PROJECT, 2024
** r-type
** File description:
** GameEngine
*/

#ifndef GAMEENGINE_HPP_
    #define GAMEENGINE_HPP_

    #include "Registry.hpp"
    #include "Components.hpp"
    #include <raylib.h>

    #define SCREEN_WIDTH 1920
    #define SCREEN_HEIGHT 1080
    #define MARGIN_LEFT 10
    #define MARGIN_TOP 10
    #define FPS_MARGIN 100
    #define WAIT_DURATION 0.016
    #define FPS_LIMIT 60

class GameEngine
{
    public:
        GameEngine(ecs::Registry &reg) : _reg(reg) {
            SetTraceLogLevel(TraceLogLevel::LOG_FATAL);
            SetConfigFlags(FLAG_WINDOW_RESIZABLE);
            InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "R-Type");
            MaximizeWindow();
        }
        ~GameEngine() = default;

        void run() {
            SetTargetFPS(FPS_LIMIT);
            while (!WindowShouldClose()) {
                BeginDrawing();
                ClearBackground(RAYWHITE);
                _reg.runSystems();
                EndDrawing();
                DrawFPS(SCREEN_WIDTH - FPS_MARGIN, MARGIN_TOP);
            }
        }

    private:
        ecs::Registry _reg;
};

#endif /* !GAMEENGINE_HPP_ */
