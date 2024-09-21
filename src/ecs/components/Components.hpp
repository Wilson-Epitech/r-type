/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** Components
*/

#ifndef COMPONENTS_HPP_
    #define COMPONENTS_HPP_

    #include <string>
    #include <memory>
    #include <raylib.h>

namespace ecs
{
    enum entityType {
        PLAYER,
        PLAYERSHOT,
        ENEMY
    };

    namespace component
    {
        struct Drawable {
            std::string spritePath;
            Texture2D spriteTexture;
            bool isLoaded = false;
        };

        struct Controllable {
            bool isControllable = true;
        };

        struct Position {
            float x, y;
        };

        struct Velocity {
            float vx, vy;
        };

        struct Type {
            ecs::entityType type;
        };

        struct Lives {
            std::size_t nbLives;
        };
    }
}

#endif /* !COMPONENTS_HPP_ */