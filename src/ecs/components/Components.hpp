/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** Components
*/

#ifndef COMPONENTS_HPP_
#define COMPONENTS_HPP_

#include "Lib.hpp"
#include <memory>
#include <raylib.h>
#include <string>

namespace ecs {
    enum EntityType {
        PLAYER,
        PLAYERSHOT,
        ENEMYSHOT,
        ENEMY
    };

    namespace component {
        struct Drawable {
            bool isBackground = false;
            bool isLoaded = false;
            Texture2D spriteTexture = Texture2D{};
            Rectangle spriteRect = {0, 0, 0, 0};
        };

        struct SpriteInfos {
            std::string spritePath;
            std::size_t patternNb = 1;
            std::size_t firstPattern = 0;
            float scaleFactor = 1.0;
        };

        struct SoundEffect {
            std::string soundPath;
            Sound sound = Sound{};
            bool isLoaded = false;
            bool hasToBePlayed = false;
        };

        struct Controllable {
            bool isControllable = true;
        };

        struct Position {
            float x, y;
        };

        struct Velocity {
            float vx, vy, possibleVelocity = 0;
        };

        struct Type {
            ecs::EntityType type;
        };

        struct Life {
            std::size_t nbLives;
        };

        struct ShootingRate {
            int shootRate = 0;
        };

        struct EnemySpawn {
            float timeBeforeSpawn;
            int waveSize;
            int maxSpawnWidth = Lib::getScreenWidth();
            int minSpawnWidth = 0;
            int maxSpawnHeight = Lib::getScreenHeight();
            int minSpawnHeight = 0;
        };
    }
}

#endif /* !COMPONENTS_HPP_ */