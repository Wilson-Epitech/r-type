/*
** EPATECH PROJECT, 2024
** r-type
** File description:
** GameEngine
*/

#ifndef GAMEENGINE_HPP_
    #define GAMEENGINE_HPP_

    #include <raylib.h>
    #include "Registry.hpp"
    #include "Components.hpp"

    #define SCREEN_WIDTH 1920
    #define SCREEN_HEIGHT 1080
    #define MARGIN_LEFT 10
    #define MARGIN_TOP 10
    #define FPS_MARGIN 100
    #define WAIT_DURATION 0.016

class GameEngine
{
    public:
        GameEngine(ecs::Registry &reg, std::shared_ptr<KeyPressed> &key) : _reg(reg), _key(key) {
            SetTraceLogLevel(TraceLogLevel::LOG_FATAL);
            SetConfigFlags(FLAG_WINDOW_RESIZABLE);
            InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "R-Type");
            MaximizeWindow();
        }
        ~GameEngine() = default;

        void run() {
            double previousTimer = GetTime();
            double timeElapse = 0.0;
            double waitTime = 0.0;
            while (!WindowShouldClose()) {
                previousTimer = GetTime();
                waitTime = previousTimer - timeElapse;
                BeginDrawing();
                if (waitTime >= WAIT_DURATION) {
                    ClearBackground(RAYWHITE);
                    timeElapse = previousTimer;
                    *_key = KeyPressed::NO_KEY;
                    if (IsKeyDown(KEY_UP))
                        *_key = KeyPressed::UP;
                    if (IsKeyDown(KEY_DOWN))
                        *_key = KeyPressed::DOWN;
                    if (IsKeyDown(KEY_LEFT))
                        *_key = KeyPressed::LEFT;
                    if (IsKeyDown(KEY_RIGHT))
                        *_key = KeyPressed::RIGHT;
                    _reg.runSystems();
                    _reg.runDraw();
                } else {
                    ClearBackground(RAYWHITE);
                    _reg.runDraw();
                }
                EndDrawing();
                DrawFPS(SCREEN_WIDTH - FPS_MARGIN, MARGIN_TOP);
            }
        }
    private:
        ecs::Registry _reg;
        std::shared_ptr<KeyPressed> _key;
};

#endif /* !GAMEENGINE_HPP_ */
