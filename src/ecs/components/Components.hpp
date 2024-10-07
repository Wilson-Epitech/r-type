/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** Components
*/

#ifndef COMPONENTS_HPP_
#define COMPONENTS_HPP_

#include <any>
#include <list>
#include <memory>
#include <raylib.h>
#include <sstream>
#include <string>
#include <vector>

namespace ecs {
    enum EntityType {
        NOTYPE,
        PLAYER,
        PLAYER_TEMPLATE,
        PLAYERSHOT,
        PLAYERSHOT_TEMPLATE,
        ENEMY,
        ENEMY_TEMPLATE,
        ENEMYSHOT,
        ENEMYSHOT_TEMPLATE,
        BACKGROUND
    };

    enum ComponentName {
        NONE,
        POSITION,
        VELOCITY,
        TYPE,
        LIFE,
        SHOOTING,
        SPRITEINFOS,
        SOUNDEFFECT,
        MUSIC
    };

    namespace component {
        struct Position {
            float x = 0, y = 0;

            bool operator==(const Position &other) const {
                return x == other.x && y == other.y;
            }
        };

        struct Velocity {
            float vx = 0, vy = 0, possibleVelocity = 0;

            bool operator==(const Velocity &other) const {
                return (vx == other.vx && vy == other.vy && possibleVelocity == other.possibleVelocity);
            }
        };

        struct Type {
            ecs::EntityType entityType = ecs::EntityType::NOTYPE;

            bool operator==(const Type &other) const {
                return (entityType == other.entityType);
            }
        };

        struct Life {
            std::size_t nbLives;

            bool operator==(const Life &other) const {
                return (nbLives == other.nbLives);
            }
        };

        struct Shooting {
            bool isShooting = false;

            bool operator==(const Shooting &other) const {
                return (isShooting == other.isShooting);
            }
        };

        struct SpriteInfos {
            std::string spritePath;
            std::size_t patternNb = 1;
            std::size_t firstPattern = 0;
            float scaleFactor = 1.0;

            bool operator==(const SpriteInfos &other) const {
                return (spritePath == other.spritePath && patternNb == other.patternNb && firstPattern == other.firstPattern &&
                        scaleFactor == other.scaleFactor);
            }
        };

        struct SoundEffect {
            std::string soundPath;
            bool isLoaded = false;
            bool hasToBePlayed = false;
            Sound sound = Sound{};

            bool operator==(const SoundEffect &other) const {
                return (soundPath == other.soundPath && isLoaded == other.isLoaded && hasToBePlayed == other.hasToBePlayed);
            }
        };

        struct Drawable {
            bool isLoaded = false;
            Texture2D spriteTexture = Texture2D{};
            Rectangle spriteRect = Rectangle{};
        };

        struct SpriteSize {
            float spriteWidth = 0, spriteHeight = 0;
        };

        struct EnemySpawn {
            float timeBeforeSpawn;
            int waveSize;
            int maxSpawnWidth = 1920;
            int minSpawnWidth = 0;
            int maxSpawnHeight = 1080;
            int minSpawnHeight = 0;
        };

        struct ShootingRate {
            int shootRate = 0;
        };

        struct BackgroundMusic {
            std::string musicPath;
            float volume;
            bool hasToBePlayed = false;
            bool isLoaded = false;
            bool isCurrentlyPlayed = false;
            Music music = Music{};

            bool operator==(const BackgroundMusic &other) const {
                return (musicPath == other.musicPath && volume == other.volume && isLoaded == other.isLoaded &&
                        hasToBePlayed == other.hasToBePlayed && isCurrentlyPlayed == other.isCurrentlyPlayed);
            }
        };
    }
}

struct EntityData {
  public:
    size_t entityID = 0;
    ecs::ComponentName name;
    std::any component;

    bool operator==(const EntityData &other) const {
        if (entityID == other.entityID && name == other.name) {
            return (areComponentsEquals<ecs::component::Position>(name, ecs::ComponentName::POSITION, component, other.component) ||
                    areComponentsEquals<ecs::component::Velocity>(name, ecs::ComponentName::VELOCITY, component, other.component) ||
                    areComponentsEquals<ecs::component::Type>(name, ecs::ComponentName::TYPE, component, other.component) ||
                    areComponentsEquals<ecs::component::Life>(name, ecs::ComponentName::LIFE, component, other.component) ||
                    areComponentsEquals<ecs::component::Shooting>(name, ecs::ComponentName::SHOOTING, component, other.component) ||
                    areComponentsEquals<ecs::component::SpriteInfos>(name, ecs::ComponentName::SPRITEINFOS, component, other.component) ||
                    areComponentsEquals<ecs::component::SoundEffect>(name, ecs::ComponentName::SOUNDEFFECT, component, other.component) ||
                    areComponentsEquals<ecs::component::BackgroundMusic>(name, ecs::ComponentName::MUSIC, component, other.component));
            return true;
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

struct DataToSend {
    std::list<std::size_t> deadEntities = {};
    std::vector<EntityData> entitiesData = {};

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
void operator>>(std::istringstream &iss, EntityData &entityData);

#endif /* !COMPONENTS_HPP_ */