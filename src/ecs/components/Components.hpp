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
    namespace component
    {
        struct Drawable {
            std::string spritePath;
            Texture2D spriteTexture;
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
            std::string type;
        };

        struct Lives {
            std::size_t nbLives;
        };
    }
}

#endif /* !COMPONENTS_HPP_ */