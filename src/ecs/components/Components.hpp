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
    enum EntityType {
        PLAYER,
        PLAYERSHOT,
        ENEMY
    };

    namespace component
    {
        struct Drawable {
            std::string spritePath;
            std::size_t patternNb = 1;
            std::size_t firstPattern = 0;
            float scaleFactor = 1.0;
            bool isBackground = false;

            bool isLoaded = false;
            Texture2D spriteTexture;
            Rectangle spriteRect;
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
            ecs::EntityType type;
        };

        struct Life {
            std::size_t nbLives;
        };
    }
}

#endif /* !COMPONENTS_HPP_ */