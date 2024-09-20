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

namespace rtype {
    enum KeyPressed {
        NO_KEY,
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
}

namespace component {

    struct Drawable {
        std::string sprite_path;
        Texture2D sprite_texture;
    };

    struct Controllable {
        std::shared_ptr<rtype::KeyPressed> key;
    };

    struct Position {
        float x, y;
    };

    struct Velocity {
        float vx, vy;
    };
}

#endif /* !COMPONENTS_HPP_ */