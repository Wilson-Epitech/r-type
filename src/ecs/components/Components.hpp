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
        POWERUP_TEXT_TEMPLATE ///< Template for the powerup text component
    };

    /**
     * @enum Poweruptype
     * @brief Enum representing different powerup types.
     */
    enum Poweruptype {
        NOPOWERUP,          ///< No powerup, must be at the end of the enum
        LIFE_POWERUP,       ///< Powerup type, give one pv to the player
        POWERSHOOT_POWERUP, ///< Powerup type, player shoot are more powerfull
        INVINCIBLE_POWERUP, ///< Powerup type, make the player invincible
        MEGASHOOT_POWERUP,  ///< Powerup type, make the player shoot twice it normal size
        END_POWERUP_TYPE    ///< End of the powerup type enum
    };

    namespace component {

        /**
         * @struct Position
         * @brief Component for the position of an entity in 2D space.
         */
        struct Position {
            float x = 0, y = 0; ///< X and Y coordinates of the entity

            bool operator==(const Position &other) const {
                return x == other.x && y == other.y;
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
            std::string spritePath;       ///< Path to the sprite file
            std::size_t patternNb = 1;    ///< Number of patterns in the sprite
            std::size_t firstPattern = 0; ///< First pattern in the sprite
            float scaleFactor = 1.0;      ///< Scale factor for the sprite
            Color color = RAYWHITE;       ///< Color applied to the sprite

            bool operator==(const SpriteInfos &other) const {
                return (spritePath == other.spritePath && patternNb == other.patternNb && firstPattern == other.firstPattern &&
                        scaleFactor == other.scaleFactor && color.a == other.color.a && color.r == other.color.r &&
                        color.g == other.color.g && color.b == other.color.b);
            }
        };

        /**
         * @struct SoundEffect
         * @brief Component for sound effects related to an entity.
         */
        struct SoundEffect {
            std::string soundPath;      ///< Path to the sound file
            bool isLoaded = false;      ///< Boolean indicating if the sound is loaded
            bool hasToBePlayed = false; ///< Boolean indicating if the sound should be played
            Sound sound = Sound{};      ///< Sound object

            bool operator==(const SoundEffect &other) const {
                return (soundPath == other.soundPath && isLoaded == other.isLoaded && hasToBePlayed == other.hasToBePlayed);
            }
        };

        /**
         * @struct Drawable
         * @brief Component representing the drawable texture of an entity.
         */
        struct Drawable {
            bool isLoaded = false;                 ///< Indicates if the texture is loaded
            Texture2D spriteTexture = Texture2D{}; ///< Texture for the entity
            Rectangle spriteRect = Rectangle{};    ///< Rectangle of the texture to be drawn
        };

        /**
         * @struct SpriteSize
         * @brief Component for the size of the sprite.
         */
        struct SpriteSize {
            float spriteWidth = 0, spriteHeight = 0; ///< Width and height of the sprite
        };

        /**
         * @struct EnemySpawn
         * @brief Component representing the enemy spawn information.
         */
        struct EnemySpawn {
            float timeBeforeSpawn;                       ///< Time before the next spawn
            int waveSize;                                ///< Size of the enemy wave to spawn
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
         * @struct BackgroundMusic
         * @brief Component for background music of an entity.
         */
        struct BackgroundMusic {
            std::string musicPath;          ///< Path to the music file
            float volume;                   ///< Volume level of the music
            bool hasToBePlayed = false;     ///< Indicates if the music should be played
            bool isLoaded = false;          ///< Indicates if the music is loaded
            bool isCurrentlyPlayed = false; ///< Indicates if the music is currently playing
            Music music = Music{};          ///< Music object

            bool operator==(const BackgroundMusic &other) const {
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
            int score = 0; /// < Score of the entity

            bool operator==(const Score &other) const {
                return (score == other.score && scoreIncrementation == other.scoreIncrementation);
            }
        };

        /**
         * @struct TextInfos
         * @brief Component for the text information of an entity
        */
        struct TextInfos {
            std::string textToDisplay; ///< Text to display
            int x; ///< X coordinates of the text
            int y; ///< Y coordinates of the text
            int size; ///< Size of the text
            std::string fontPath; ///< Path of the font used to draw the text
            Color color = Color{}; ///< Color of the text
            Font font = Font{}; ///< Font of the text
            bool fontIsLoaded = false; ///< Indicates if font has been loaded
            bool hasChanged = false; ///< Indicates if the textToDisplay string has changed

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
            int spawnRate = 5; ///< 1 chance of spawn in spawnRate (1 / spawnRate)
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
         * @struct PowerupTextInfos
         * @brief Component for the text information of the powerup display an entity
        */
        struct PowerupTextInfos {
            int x; ///< X coordinates of the text
            int y; ///< Y coordinates of the text
            int size; ///< Size of the text
            std::string fontPath; ///< Path of the font used to draw the text
            Color color = Color{}; ///< Color of the text
            Font font = Font{}; ///< Font of the text
            bool fontIsLoaded = false; ///< Indicates if font has been loaded

            bool operator==(const PowerupTextInfos &other) const {
                return (x == other.x && y == other.y && size == other.size &&
                        fontPath == other.fontPath && color.a == other.color.a && color.r == other.color.r && color.g == other.color.g &&
                        color.b == other.color.b);
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
            return (areComponentsEquals<ecs::component::Position>(name, ecs::ComponentName::POSITION, component, other.component) ||
                    areComponentsEquals<ecs::component::Velocity>(name, ecs::ComponentName::VELOCITY, component, other.component) ||
                    areComponentsEquals<ecs::component::Type>(name, ecs::ComponentName::TYPE, component, other.component) ||
                    areComponentsEquals<ecs::component::Life>(name, ecs::ComponentName::LIFE, component, other.component) ||
                    areComponentsEquals<ecs::component::Shooting>(name, ecs::ComponentName::SHOOTING, component, other.component) ||
                    areComponentsEquals<ecs::component::SpriteInfos>(name, ecs::ComponentName::SPRITEINFOS, component, other.component) ||
                    areComponentsEquals<ecs::component::SoundEffect>(name, ecs::ComponentName::SOUNDEFFECT, component, other.component) ||
                    areComponentsEquals<ecs::component::ActivePowerup>(name, ecs::ComponentName::ACTIVEPOWERUP, component, other.component) ||
                    areComponentsEquals<ecs::component::PowerupTextInfos>(name, ecs::ComponentName::POWERUP_TEXT_TEMPLATE, component, other.component) ||
                    areComponentsEquals<ecs::component::BackgroundMusic>(name, ecs::ComponentName::MUSIC, component, other.component));
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
void operator<<(std::ostringstream &oss, const ecs::component::SoundEffect &soundEffect);
void operator<<(std::ostringstream &oss, const ecs::component::BackgroundMusic &music);
void operator<<(std::ostringstream &oss, const ecs::component::TextInfos &textInfo);
void operator<<(std::ostringstream &oss, const ecs::component::Score &score);
void operator<<(std::ostringstream &oss, const ecs::component::ActivePowerup &activePowerup);
void operator<<(std::ostringstream &oss, const ecs::component::PowerupTextInfos &powerupTextInfos);
void operator<<(std::ostringstream &oss, const EntityData &entityData);

void operator>>(std::istringstream &iss, ecs::EntityType &type);
void operator>>(std::istringstream &iss, ecs::ComponentName &name);
void operator>>(std::istringstream &iss, ecs::component::Position &position);
void operator>>(std::istringstream &iss, ecs::component::Velocity &velocity);
void operator>>(std::istringstream &iss, ecs::component::SpriteInfos &spriteInfos);
void operator>>(std::istringstream &iss, ecs::component::Shooting &shooting);
void operator>>(std::istringstream &iss, ecs::component::Type &type);
void operator>>(std::istringstream &iss, ecs::component::Life &life);
void operator>>(std::istringstream &iss, ecs::component::SoundEffect &soundEffect);
void operator>>(std::istringstream &iss, ecs::component::BackgroundMusic &music);
void operator>>(std::istringstream &iss, ecs::component::TextInfos &textInfo);
void operator>>(std::istringstream &iss, ecs::component::Score &score);
void operator>>(std::istringstream &iss, ecs::component::ActivePowerup &activePowerup);
void operator>>(std::istringstream &iss, ecs::component::PowerupTextInfos &powerupTextInfos);
void operator>>(std::istringstream &iss, EntityData &entityData);

#endif /* !COMPONENTS_HPP_ */