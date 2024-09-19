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
        std::shared_ptr<component::key> keyPressed;
    };

    struct Position {
        float x, y;
    };

    struct Velocity {
        float vx, vy;
    };
}

void draw_system(registry& reg, sparse_array<component::Drawable> const &drawables,
    sparse_array<component::Position> const &positions)
{
    for (std::size_t i = 0; i < drawables.size() && i < positions.size(); ++i) {
        if (drawables[i] && positions[i]) {
            if (drawables[i]->spritePath.length() > 0) {
                DrawTexture(drawables[i]->texture, positions[i]->x, positions[i]->y, RAYWHITE);
            }
        }
    }
}

void control_system(registry& reg, sparse_array<component::Controllable> const &controllables,
    sparse_array<component::Velocity> &velocities)
{
    for (std::size_t i = 0; i < controllables.size() && i < velocities.size(); ++i) {
        if (controllables[i] && velocities[i]) {
            if (*controllables[i]->keyPressed == component::key::UP) {
                velocities[i]->vx = 0;
                velocities[i]->vy = -0.2;
            } else if (*controllables[i]->keyPressed == component::key::DOWN) {
                velocities[i]->vx = 0;
                velocities[i]->vy = 0.2;
            } else if (*controllables[i]->keyPressed == component::key::LEFT) {
                velocities[i]->vx = -0.2;
                velocities[i]->vy = 0;
            } else if (*controllables[i]->keyPressed == component::key::RIGHT) {
                velocities[i]->vx = 0.2;
                velocities[i]->vy = 0;
            }
        }
    }
}

void position_system(registry& reg, sparse_array<component::Position> &positions,
    sparse_array<component::Velocity> const &velocities)
{
    for (std::size_t i = 0; i < positions.size() && i < velocities.size(); ++i) {
        if (positions[i] && velocities[i]) {
            positions[i]->x += velocities[i]->vx;
            positions[i]->y += velocities[i]->vy;
        }
    }
}

void load_all_textures(registry& reg)
{
    auto& drawables = reg.get_components<component::Drawable>();

    for (std::size_t i = 0; i < drawables.size(); ++i) {
        if (drawables[i]) {
            if (drawables[i]->spritePath.length() > 0) {
                drawables[i]->texture = LoadTexture(drawables[i]->spritePath.c_str());
            }
        }
    }
}

int main()
{
    registry reg;
    std::shared_ptr<component::key> keyPressed = std::make_shared<component::key>(component::key::NO_KEY);

    auto &position_array = reg.register_component<component::Position>();
    auto &velocity_array = reg.register_component<component::Velocity>();
    auto &drawable_array = reg.register_component<component::Drawable>();
    auto &controllable_array = reg.register_component<component::Controllable>();

    entity_t movableEntity1 = reg.spawn_entity();
    entity_t movableEntity2 = reg.spawn_entity();
    entity_t player = reg.spawn_entity();

    position_array.insert_at(player, component::Position{100, 100});
    velocity_array.insert_at(player, component::Velocity{0, 0});
    controllable_array.insert_at(player, component::Controllable{keyPressed});
    drawable_array.insert_at(player, component::Drawable{"../assets/r-typesheet39.gif"});

    position_array.insert_at(movableEntity1, component::Position{1500, 700});
    velocity_array.insert_at(movableEntity1, component::Velocity{-0.4f, 0});
    drawable_array.insert_at(movableEntity1, component::Drawable{"../assets/r-typesheet30a.gif"});

    position_array.insert_at(movableEntity2, component::Position{2000, 300});
    velocity_array.insert_at(movableEntity2, component::Velocity{-0.3f, 0});
    drawable_array.insert_at(movableEntity2, component::Drawable{"../assets/r-typesheet30a.gif"});


    SetTraceLogLevel(TraceLogLevel::LOG_FATAL);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "R-Type");
    MaximizeWindow();

    load_all_textures(reg);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        if (IsKeyPressed(KEY_UP))
            *keyPressed = component::key::UP;
        if (IsKeyPressed(KEY_DOWN))
            *keyPressed = component::key::DOWN;
        if (IsKeyPressed(KEY_LEFT))
            *keyPressed = component::key::LEFT;
        if (IsKeyPressed(KEY_RIGHT))
            *keyPressed = component::key::RIGHT;
        control_system(reg, controllable_array, velocity_array);
        position_system(reg, position_array, velocity_array);
        draw_system(reg, drawable_array, position_array);
        DrawFPS(SCREEN_WIDTH - FPS_MARGIN, MARGIN_TOP);
        EndDrawing();
    }
    return 0;
}
