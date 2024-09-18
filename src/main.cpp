/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** main
*/

#include <iostream>
#include <memory>
#include "Registry.hpp"

/**
 * @brief this is the main function,
 * ...
 * @return int
 */

namespace component {

    enum key {
        NO_KEY,
        UP,
        DOWN,
        LEFT,
        RIGHT
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

void control_system(registry& reg)
{
    auto& controllables = reg.get_components<component::Controllable>();
    auto& velocities = reg.get_components<component::Velocity>();

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

void position_system(registry& reg)
{
    auto& positions = reg.get_components<component::Position>();
    auto& velocities = reg.get_components<component::Velocity>();

    for (std::size_t i = 0; i < positions.size() && i < velocities.size(); ++i) {
        if (positions[i] && velocities[i]) {
            positions[i]->x += velocities[i]->vx;
            positions[i]->y += velocities[i]->vy;
        }
    }
}

int main()
{
    return 0;
}
