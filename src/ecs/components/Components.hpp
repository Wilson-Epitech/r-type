/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** Components
*/

/**
 * @file Components.hpp
 * @brief Defines components used in the ECS system for the game engine.
 * This file contains the declaration of various components used in the ECS system,
 * such as position, velocity, type, life, sprite information, sound effects, and others.
 */

#ifndef COMPONENTS_HPP_
#define COMPONENTS_HPP_

#include "Lib.hpp"
#include <any>
#include <list>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#define ORANGE_COLOR                                                                                                                       \
    (ecs::CustomColor) { 255, 161, 0, 255 } ///< Orange color
#define CYAN_COLOR                                                                                                                         \
    (ecs::CustomColor) { 120, 200, 225, 255 } ///< Cyan color
#define BEIGE_COLOR                                                                                                                        \
    (ecs::CustomColor) { 211, 176, 131, 255 } ///< Beige color
#define GOLD_COLOR                                                                                                                         \
    (ecs::CustomColor) { 255, 203, 0, 255 } ///< Gold color
#define PINK_COLOR                                                                                                                         \
    (ecs::CustomColor) { 255, 109, 194, 255 } ///< Pink color
#define WHITE_COLOR                                                                                                                        \
    (ecs::CustomColor) { 245, 245, 245, 255 } ///< White color

namespace ecs {

    /**
     * @enum EntityType
     * @brief Enum representing different types of entities in the game.
     */
    enum EntityType {
        NOTYPE,              ///< No type entity
        PLAYER,              ///< Player entity
        PLAYER_TEMPLATE,     ///< Player template entity
        PLAYERSHOT,          ///< Player's shot entity
        PLAYERSHOT_TEMPLATE, ///< Player's shot template entity
        ENEMY,               ///< Enemy entity
        ENEMY_TEMPLATE,      ///< Enemy template entity
        ENEMYSHOT,           ///< Enemy's shot entity
        ENEMYSHOT_TEMPLATE,  ///< Enemy's shot template entity
        BACKGROUND,          ///< Background entity
        TEXT,                ///< Text entity
        POWERUP,             ///< Powerup entity
        POWERUP_TEMPLATE     ///< Powerup template entity
    };

    /**
     * @enum ComponentName
     * @brief Enum representing different component types in the ECS.
     */
    enum ComponentName {
        NONE,           ///< No component
        POSITION,       ///< Position component
        VELOCITY,       ///< Velocity component
        TYPE,           ///< Type component
        LIFE,           ///< Life component
        SHOOTING,       ///< Shooting component
        SPRITEINFOS,    ///< Sprite information component
        SOUNDEFFECT,    ///< Sound effect component
        MUSIC,          ///< Background music component
        SCORE,          ///< Score component
        TEXTINFOS,      ///< Text component
        ACTIVEPOWERUP,  ///< Activated powerup component
        MOVEMENTPATTERN  ///< Movement pattern component
    };

    /**
     * @enum Poweruptype
     * @brief Enum representing different powerup types.
     */
    enum Poweruptype {
        NOPOWERUP,          ///< No powerup
        LIFE_POWERUP,       ///< Powerup type, give one pv to the player
        POWERSHOOT_POWERUP, ///< Powerup type, player shoot are more powerfull
        INVINCIBLE_POWERUP, ///< Powerup type, make the player invincible
        MEGASHOOT_POWERUP,  ///< Powerup type, make the player shoot twice it normal size
        END_POWERUP_TYPE    ///< End of the powerup type enum
    };

    /**
     * @enum MovementType
     * @brief Enum representing different movement pattern types.
     */
    enum MovementType {
        STRAIGHT,   ///< Straight movement
        SNAKE       ///< Snake movement
    };

    namespace component {

        /**
         * @struct Position
         * @brief Component for the position of an entity in 2D space.
         */
        struct Position {
            float x = 0, y = 0; ///< X and Y coordinates of the entity
            float startX = 0, startY = 0; ///< X and Y coordinates of the entity before every movement

            bool operator==(const Position &other) const {
                return x == other.x && y == other.y && startX == other.startX && startY == other.startY;
            }
        };

        /**
         * @struct Velocity
         * @brief Component representing the velocity of an entity.
         */
        struct Velocity {
            float vx = 0, vy = 0, possibleVelocity = 0; ///< Velocity in the X and Y directions, and possible max velocity

            bool operator==(const Velocity &other) const {
                return (vx == other.vx && vy == other.vy && possibleVelocity == other.possibleVelocity);
            }
        };

        /**
         * @struct Type
         * @brief Component representing the type of the entity.
         */
        struct Type {
            ecs::EntityType entityType = ecs::EntityType::NOTYPE; ///< The type of the entity

            bool operator==(const Type &other) const {
                return (entityType == other.entityType);
            }
        };

        /**
         * @struct Life
         * @brief Component representing the life count of an entity.
         */
        struct Life {
            std::size_t nbLives = 1;      ///< Number of lives the entity has
            bool displayLife = false; ///< Indicates if the life has to be displayed
            bool hasToSummonPowerup = false; ///< Indicates if a powerup has a chance to spawn after death
            std::size_t maxLives = 0; ///< Max number of lives an entity has, takes the nbLives value when the entity spawn
            bool isInvincible = false; ///< Indicates if the entity is invincible

            bool operator==(const Life &other) const {
                return (nbLives == other.nbLives && displayLife == other.displayLife && maxLives == other.maxLives && isInvincible == other.isInvincible);
            }
        };

        /**
         * @struct Shooting
         * @brief Component that determines whether the entity is currently shooting.
         */
        struct Shooting {
            bool isShooting = false; ///< Boolean indicating if the entity is shooting
            std::size_t shootPower = 1; ///< How much damage do the shoot
            float shootSize = 1;    ///< Size of the shoot

            bool operator==(const Shooting &other) const {
                return (isShooting == other.isShooting && shootPower == other.shootPower && shootSize == other.shootSize);
            }
        };

        /**
         * @struct SpriteInfos
         * @brief Component for sprite information of an entity.
         */
        struct SpriteInfos {
            std::string spritePath;                        ///< Path to the sprite file
            std::size_t patternNb = 1;                     ///< Number of patterns in the sprite
            std::size_t firstPattern = 0;                  ///< First pattern in the sprite
            float scaleFactor = 1.0;                       ///< Scale factor for the sprite
            CustomColor color = WHITE_COLOR;               ///< Color applied to the sprite, white by default
            std::size_t spriteWidth = 0, spriteHeight = 0; ///< Width and height of the sprite

            bool operator==(const SpriteInfos &other) const {
                return (spritePath == other.spritePath && patternNb == other.patternNb && firstPattern == other.firstPattern &&
                        scaleFactor == other.scaleFactor && color.a == other.color.a && color.r == other.color.r &&
                        color.g == other.color.g && color.b == other.color.b && spriteWidth == other.spriteWidth &&
                        spriteHeight == other.spriteHeight);
            }
        };

        /**
         * @struct SoundEffectInfos
         * @brief Component for sound effects informations related to an entity.
         */
        struct SoundEffectInfos {
            std::string soundPath;      ///< Path to the sound file
            bool isLoaded = false;      ///< Boolean indicating if the sound is loaded
            bool hasToBePlayed = false; ///< Boolean indicating if the sound should be played

            bool operator==(const SoundEffectInfos &other) const {
                return (soundPath == other.soundPath && isLoaded == other.isLoaded && hasToBePlayed == other.hasToBePlayed);
            }
        };

        /**
         * @struct EnemySpawn
         * @brief Component representing the enemy spawn information.
         */
        struct EnemySpawn {
            float timeBeforeSpawn;                       ///< Time before the next spawn
            std::size_t waveSize;                        ///< Size of the enemy wave to spawn
            int maxSpawnWidth = Lib::getScreenWidth();   ///< Maximum spawn width
            int minSpawnWidth = 0;                       ///< Minimum spawn width
            int maxSpawnHeight = Lib::getScreenHeight(); ///< Maximum spawn height
            int minSpawnHeight = 0;                      ///< Minimum spawn height
        };

        /**
         * @struct ShootingRate
         * @brief Component representing the shooting rate of an entity.
         */
        struct ShootingRate {
            float shootRate = 0; ///< Rate of shooting
        };

        /**
         * @struct ShootPlayerIndex
         * @brief Component representing the index of the player that shot
         */
        struct ShootPlayerIndex {
            std::size_t playerIndex; ///< Index of the player that shot
        };

        /**
         * @struct BackgroundMusicInfos
         * @brief Component for background music informations of an entity.
         */
        struct BackgroundMusicInfos {
            std::string musicPath;          ///< Path to the music file
            float volume;                   ///< Volume level of the music
            bool hasToBePlayed = false;     ///< Indicates if the music should be played
            bool isLoaded = false;          ///< Indicates if the music is loaded
            bool isCurrentlyPlayed = false; ///< Indicates if the music is currently playing

            bool operator==(const BackgroundMusicInfos &other) const {
                return (musicPath == other.musicPath && volume == other.volume && isLoaded == other.isLoaded &&
                        hasToBePlayed == other.hasToBePlayed && isCurrentlyPlayed == other.isCurrentlyPlayed);
            }
        };

        /**
         * @struct Score
         * @brief Component for the score of an entity
         */
        struct Score {
            std::size_t scoreIncrementation = 0; ///< Incrementation value for score
            int score = 0;                       /// < Score of the entity

            bool operator==(const Score &other) const {
                return (score == other.score && scoreIncrementation == other.scoreIncrementation);
            }
        };

        /**
         * @struct TextInfos
         * @brief Component for text informations of an entity
         */
        struct TextInfos {
            std::string textToDisplay;                ///< Text to display
            int x;                                    ///< X coordinates of the text
            int y;                                    ///< Y coordinates of the text
            int size;                                 ///< Size of the text
            std::string fontPath;                     ///< Path of the font used to draw the text
            CustomColor color = {245, 245, 245, 255}; ///< Color of the text, white by default
            bool hasChanged = false;                  ///< Indicates if the textToDisplay string has changed
            bool fontIsLoaded = false;                ///< Indicates if font has been loaded

            bool operator==(const TextInfos &other) const {
                return (textToDisplay == other.textToDisplay && x == other.x && y == other.y && size == other.size &&
                        fontPath == other.fontPath && color.a == other.color.a && color.r == other.color.r && color.g == other.color.g &&
                        color.b == other.color.b);
            }
        };

        /**
         * @struct Powerup
         * @brief Component for the powerup information a powerup
        */
        struct Powerup {
            Poweruptype powerupType = Poweruptype::LIFE_POWERUP; ///< Type of the powerup
            std::size_t spawnRate = 5; ///< 1 chance of spawn in spawnRate (1 / spawnRate)
            bool hasBeenActivated = false;  ///< Indicates if the powerup is activated
            std::size_t playerIndex = 0;    ///< Entity of the player that have activated the powerup
        };

        /**
         * @struct ActivePowerup
         * @brief Component for the powerup that has been used by a player
        */
        struct ActivePowerup {
            bool isPowershootActive = false;    ///< Indicates if the powerup Powershoot is activated
            bool isInvincibleActive = false;    ///< Indicates if the powerup Invincible is activated
            bool isMegaShootActive = false;     ///< Indicates if the powerup MegaShoot is activated

            bool operator==(const ActivePowerup &other) const {
                return (isPowershootActive == other.isPowershootActive &&
                        isInvincibleActive == other.isInvincibleActive &&
                        isMegaShootActive == other.isMegaShootActive);
            }
        };

        /**
         * @struct MovementPattern
         * @brief Component for the movement pattern of an entity
        */
        struct MovementPattern {
            std::size_t patternAmplitude = 0;
            bool isReversible = false;
            ecs::MovementType movementPatternType = MovementType::STRAIGHT;

            bool operator==(const MovementPattern &other) const {
                return (patternAmplitude == other.patternAmplitude && isReversible == other.isReversible && movementPatternType == other.movementPatternType);
            }
        };
    }
}

/**
 * @struct EntityData
 * @brief Represents the data of an entity, including its ID and components.
 */
struct EntityData {
  public:
    size_t entityID = 0;     ///< Unique ID of the entity
    ecs::ComponentName name; ///< The name of the component
    std::any component;      ///< The component data stored as std::any

    bool operator==(const EntityData &other) const {
        if (entityID == other.entityID && name == other.name) {
            return (
                areComponentsEquals<ecs::component::Position>(name, ecs::ComponentName::POSITION, component, other.component) ||
                areComponentsEquals<ecs::component::Velocity>(name, ecs::ComponentName::VELOCITY, component, other.component) ||
                areComponentsEquals<ecs::component::Type>(name, ecs::ComponentName::TYPE, component, other.component) ||
                areComponentsEquals<ecs::component::Life>(name, ecs::ComponentName::LIFE, component, other.component) ||
                areComponentsEquals<ecs::component::Shooting>(name, ecs::ComponentName::SHOOTING, component, other.component) ||
                areComponentsEquals<ecs::component::SpriteInfos>(name, ecs::ComponentName::SPRITEINFOS, component, other.component) ||
                areComponentsEquals<ecs::component::SoundEffectInfos>(name, ecs::ComponentName::SOUNDEFFECT, component, other.component) ||
                areComponentsEquals<ecs::component::TextInfos>(name, ecs::ComponentName::TEXTINFOS, component, other.component) ||
                areComponentsEquals<ecs::component::ActivePowerup>(name, ecs::ComponentName::ACTIVEPOWERUP, component, other.component) ||
                areComponentsEquals<ecs::component::BackgroundMusicInfos>(name, ecs::ComponentName::MUSIC, component, other.component) ||
                areComponentsEquals<ecs::component::Score>(name, ecs::ComponentName::SCORE, component, other.component) ||
                areComponentsEquals<ecs::component::MovementPattern>(name, ecs::ComponentName::MOVEMENTPATTERN, component, other.component));
        }
        return false;
    }

  private:
    template <typename Component>
    bool areComponentsEquals(const ecs::ComponentName componentName, const ecs::ComponentName name, std::any component,
                             std::any otherComponent) const {
        if (componentName == name) {
            auto comp = std::any_cast<Component>(component);
            auto otherComp = std::any_cast<Component>(otherComponent);
            if (comp == otherComp)
                return true;
        }
        return false;
    }
};

/**
 * @struct DataToSend
 * @brief Structure for holding data to send between systems.
 */
struct DataToSend {
    std::list<std::size_t> deadEntities = {};  ///< List of dead entities to be removed
    std::vector<EntityData> entitiesData = {}; ///< List of entities' data

    bool empty() {
        return (deadEntities.empty() && entitiesData.empty());
    }
    bool operator==(const DataToSend &other) const {
        return (deadEntities == other.deadEntities && entitiesData == other.entitiesData);
    }
};

void operator<<(std::ostringstream &oss, const ecs::component::Position &position);
void operator<<(std::ostringstream &oss, const ecs::component::Velocity &velocity);
void operator<<(std::ostringstream &oss, const ecs::component::SpriteInfos &spriteInfos);
void operator<<(std::ostringstream &oss, const ecs::component::Shooting &shooting);
void operator<<(std::ostringstream &oss, const ecs::component::Type &type);
void operator<<(std::ostringstream &oss, const ecs::component::Life &life);
void operator<<(std::ostringstream &oss, const ecs::component::SoundEffectInfos &soundEffectInfos);
void operator<<(std::ostringstream &oss, const ecs::component::BackgroundMusicInfos &music);
void operator<<(std::ostringstream &oss, const ecs::component::TextInfos &textInfo);
void operator<<(std::ostringstream &oss, const ecs::component::Score &score);
void operator<<(std::ostringstream &oss, const ecs::component::ActivePowerup &activePowerup);
void operator<<(std::ostringstream &oss, const ecs::component::MovementPattern &movementPattern);
void operator<<(std::ostringstream &oss, const EntityData &entityData);

void operator>>(std::istringstream &iss, ecs::EntityType &type);
void operator>>(std::istringstream &iss, ecs::ComponentName &name);
void operator>>(std::istringstream &iss, ecs::component::Position &position);
void operator>>(std::istringstream &iss, ecs::component::Velocity &velocity);
void operator>>(std::istringstream &iss, ecs::component::SpriteInfos &spriteInfos);
void operator>>(std::istringstream &iss, ecs::component::Shooting &shooting);
void operator>>(std::istringstream &iss, ecs::component::Type &type);
void operator>>(std::istringstream &iss, ecs::component::Life &life);
void operator>>(std::istringstream &iss, ecs::component::SoundEffectInfos &soundEffectInfos);
void operator>>(std::istringstream &iss, ecs::component::BackgroundMusicInfos &music);
void operator>>(std::istringstream &iss, ecs::component::TextInfos &textInfo);
void operator>>(std::istringstream &iss, ecs::component::Score &score);
void operator>>(std::istringstream &iss, ecs::component::ActivePowerup &activePowerup);
void operator>>(std::istringstream &iss, ecs::component::MovementPattern &movementPattern);
void operator>>(std::istringstream &iss, EntityData &entityData);

#endif /* !COMPONENTS_HPP_ */