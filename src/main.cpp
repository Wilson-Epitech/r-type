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
    registry reg;
    std::shared_ptr<rtype::KeyPressed> key = std::make_shared<rtype::KeyPressed>(rtype::KeyPressed::NO_KEY);

    auto &position_array = reg.register_component<component::Position>();
    auto &velocity_array = reg.register_component<component::Velocity>();
    auto &drawable_array = reg.register_component<component::Drawable>();
    auto &controllable_array = reg.register_component<component::Controllable>();

    entity_t player = reg.spawn_entity();
    entity_t movable_entity1 = reg.spawn_entity();
    entity_t movable_entity2 = reg.spawn_entity();

    position_array.insert_at(player, component::Position{100, 100});
    velocity_array.insert_at(player, component::Velocity{0, 0});
    controllable_array.insert_at(player, component::Controllable{key});
    drawable_array.insert_at(player, component::Drawable{"../assets/r-typesheet39.gif"});

    position_array.insert_at(movable_entity1, component::Position{1500, 700});
    velocity_array.insert_at(movable_entity1, component::Velocity{-0.4f, 0});
    drawable_array.insert_at(movable_entity1, component::Drawable{"../assets/r-typesheet30a.gif"});

    position_array.insert_at(movable_entity2, component::Position{2000, 300});
    velocity_array.insert_at(movable_entity2, component::Velocity{-0.3f, 0});
    drawable_array.insert_at(movable_entity2, component::Drawable{"../assets/r-typesheet30a.gif"});

    SetTraceLogLevel(TraceLogLevel::LOG_FATAL);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "R-Type");
    MaximizeWindow();

    reg.add_system(rtype::ControlSystem());
    reg.add_system(rtype::PositionSystem());
    reg.add_system(rtype::DrawSystem());

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        if (IsKeyPressed(KEY_UP))
            *key = rtype::KeyPressed::UP;
        if (IsKeyPressed(KEY_DOWN))
            *key = rtype::KeyPressed::DOWN;
        if (IsKeyPressed(KEY_LEFT))
            *key = rtype::KeyPressed::LEFT;
        if (IsKeyPressed(KEY_RIGHT))
            *key = rtype::KeyPressed::RIGHT;
        reg.run_systems();
        DrawFPS(SCREEN_WIDTH - FPS_MARGIN, MARGIN_TOP);
        EndDrawing();
    }
    return 0;
}
