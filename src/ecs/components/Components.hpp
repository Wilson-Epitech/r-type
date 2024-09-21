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

enum KeyPressed
{
    NO_KEY,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    SPACE
};

namespace ecs
{
    namespace component
    {
        struct Drawable {
            std::string spritePath;
            Texture2D spriteTexture;
            bool isLoaded = false;
        };

        struct Controllable {
            std::shared_ptr<KeyPressed> key;
        };

        struct Position {
            float x, y;
        };

        struct Velocity {
            float vx, vy;
        };

        struct Type {
            std::string type;
        };

        struct Lives {
            std::size_t nbLives;
        };
    }
}

#endif /* !COMPONENTS_HPP_ */