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

#include "ControlSystem.hpp"
#include "PositionSystem.hpp"
#include "DrawSystem.hpp"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define MARGIN_LEFT 10
#define MARGIN_TOP 10
#define FPS_MARGIN 100

int main()
{
    ecs::Registry reg;
    std::shared_ptr<KeyPressed> key = std::make_shared<KeyPressed>(KeyPressed::NO_KEY);

    auto &positionArray = reg.registerComponent<ecs::component::Position>();
    auto &velocityArray = reg.registerComponent<ecs::component::Velocity>();
    auto &drawableArray = reg.registerComponent<ecs::component::Drawable>();
    auto &ControllableArray = reg.registerComponent<ecs::component::Controllable>();

    entity_t player = reg.spawnEntity();
    entity_t movableEntity1 = reg.spawnEntity();
    entity_t movableEntity2 = reg.spawnEntity();

    positionArray.insertAt(player, ecs::component::Position{100, 100});
    velocityArray.insertAt(player, ecs::component::Velocity{0, 0});
    ControllableArray.insertAt(player, ecs::component::Controllable{key});
    drawableArray.insertAt(player, ecs::component::Drawable{"../assets/r-typesheet39.gif"});

    positionArray.insertAt(movableEntity1, ecs::component::Position{1500, 700});
    velocityArray.insertAt(movableEntity1, ecs::component::Velocity{-0.4f, 0});
    drawableArray.insertAt(movableEntity1, ecs::component::Drawable{"../assets/r-typesheet30a.gif"});

    positionArray.insertAt(movableEntity2, ecs::component::Position{2000, 300});
    velocityArray.insertAt(movableEntity2, ecs::component::Velocity{-0.3f, 0});
    drawableArray.insertAt(movableEntity2, ecs::component::Drawable{"../assets/r-typesheet30a.gif"});

    SetTraceLogLevel(TraceLogLevel::LOG_FATAL);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "R-Type");
    MaximizeWindow();

    reg.addSystem(ecs::system::ControlSystem());
    reg.addSystem(ecs::system::PositionSystem());
    reg.addSystem(ecs::system::DrawSystem());

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
