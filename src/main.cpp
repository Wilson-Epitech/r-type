/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** main
*/

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <iostream>
#include <memory>
#include "Registry.hpp"

/**
 * @brief this is the main function,
 * ...
 * @return int
 */

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define MARGIN_LEFT 10
#define MARGIN_TOP 10
#define FPS_MARGIN 100

namespace component {

    enum key {
        NO_KEY,
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    struct Drawable {
        std::string spritePath;
        Texture2D texture;
    };

    struct Controllable {
        std::shared_ptr<component::key> key_pressed;
    };

    struct Position {
        float x, y;
    };

    struct Velocity {
        float vx, vy;
    };
}

void load_all_textures(sparse_array<component::Drawable> &drawables)
{
    static bool is_loaded = false;

    if (is_loaded)
        return;
    for (std::size_t i = 0; i < drawables.size(); ++i) {
        if (drawables[i]) {
            if (drawables[i]->spritePath.length() > 0) {
                drawables[i]->texture = LoadTexture(drawables[i]->spritePath.c_str());
            }
        }
    }
    is_loaded = true;
}

void draw_system(sparse_array<component::Drawable> &drawables,
    sparse_array<component::Position> const &positions)
{
    load_all_textures(drawables);

    for (std::size_t i = 0; i < drawables.size() && i < positions.size(); ++i) {
        if (drawables[i] && positions[i]) {
            if (drawables[i]->spritePath.length() > 0) {
                DrawTexture(drawables[i]->texture, positions[i]->x, positions[i]->y, RAYWHITE);
            }
        }
    }
}

void control_system(sparse_array<component::Controllable> const &controllables,
    sparse_array<component::Velocity> &velocities)
{
    for (std::size_t i = 0; i < controllables.size() && i < velocities.size(); ++i) {
        if (controllables[i] && velocities[i]) {
            if (*controllables[i]->key_pressed == component::key::UP) {
                velocities[i]->vx = 0;
                velocities[i]->vy = -0.2;
            } else if (*controllables[i]->key_pressed == component::key::DOWN) {
                velocities[i]->vx = 0;
                velocities[i]->vy = 0.2;
            } else if (*controllables[i]->key_pressed == component::key::LEFT) {
                velocities[i]->vx = -0.2;
                velocities[i]->vy = 0;
            } else if (*controllables[i]->key_pressed == component::key::RIGHT) {
                velocities[i]->vx = 0.2;
                velocities[i]->vy = 0;
            }
        }
    }
}

void position_system(sparse_array<component::Position> &positions,
    sparse_array<component::Velocity> const &velocities)
{
    for (std::size_t i = 0; i < positions.size() && i < velocities.size(); ++i) {
        if (positions[i] && velocities[i]) {
            positions[i]->x += velocities[i]->vx;
            positions[i]->y += velocities[i]->vy;
        }
    }
}

int main()
{
    registry reg;
    std::shared_ptr<component::key> key_pressed = std::make_shared<component::key>(component::key::NO_KEY);

    auto &position_array = reg.register_component<component::Position>();
    auto &velocity_array = reg.register_component<component::Velocity>();
    auto &drawable_array = reg.register_component<component::Drawable>();
    auto &controllable_array = reg.register_component<component::Controllable>();

    entity_t player = reg.spawn_entity();
    entity_t movable_entity1 = reg.spawn_entity();
    entity_t movable_entity2 = reg.spawn_entity();

    position_array.insert_at(player, component::Position{100, 100});
    velocity_array.insert_at(player, component::Velocity{0, 0});
    controllable_array.insert_at(player, component::Controllable{key_pressed});
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

    reg.add_system<component::Controllable, component::Velocity>(control_system);
    reg.add_system<component::Position, component::Velocity>(position_system);
    reg.add_system<component::Drawable, component::Position>(draw_system);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        if (IsKeyPressed(KEY_UP))
            *key_pressed = component::key::UP;
        if (IsKeyPressed(KEY_DOWN))
            *key_pressed = component::key::DOWN;
        if (IsKeyPressed(KEY_LEFT))
            *key_pressed = component::key::LEFT;
        if (IsKeyPressed(KEY_RIGHT))
            *key_pressed = component::key::RIGHT;
        reg.run_systems();
        DrawFPS(SCREEN_WIDTH - FPS_MARGIN, MARGIN_TOP);
        EndDrawing();
    }
    return 0;
}
