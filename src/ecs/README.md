
---

# Game Engine Documentation

## Introduction

This project implements a game engine based on the **ECS** (Entity-Component-System) architecture for the **R-Type** game. ECS allows flexible and efficient management of game entities, where each entity is composed of multiple components and is managed by systems that manipulate these components.

## Engine Structure

Our game engine is composed of three main elements in the ECS architecture:
- **Entities**: Represent game objects (players, enemies, projectiles, etc.).
- **Components**: Store the data associated with entities.
- **Systems**: Contain the logic to manipulate entities through their components.

## Sumary

1. [Components](#Components)
2. [How to add a new component](#AddComponents)
3. [Systems](#Systems)
4. [Registry](#Registry)
5. [ECS Architecture](#ECSArchitecture)

## Components
> [!NOTE]
> Every components that are sent by/to the server have an overload operator "==" to check if the component has been modified

<div id='Components'/>

**Components** are structures used to store the data of an entity. Below is an overview of the available components:

### Type and Name
There is two enum in the Components.hpp.
The first one is the EntityType
```hpp
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
   BACKGROUND,
   TEXT
};
```
The seconde one is to store the name of our components
```hpp
enum ComponentName {
   NONE,
   POSITION,
   VELOCITY,
   TYPE,
   LIFE,
   SHOOTING,
   SPRITEINFOS,
   SOUNDEFFECT,
   MUSIC,
   SCORE,
   TEXTINFOS
};
```

### Every curently available components

1. **Position**
   - Store the position (x, y) of the entity
   ```c++
   struct Position {
      float x = 0, y = 0;

      bool operator==(const Position &other) const {
         return x == other.x && y == other.y;
      }
   };
      ```
2. **Velocity**
   - Store the velocity and the possible velocity of the entity<br>
   The velocity is used to define the speed of the entity
   ```c++
   struct Velocity {
      float vx = 0, vy = 0, possibleVelocity = 0;

      bool operator==(const Velocity &other) const {
         return (vx == other.vx && vy == other.vy && possibleVelocity == other.possibleVelocity);
      }
   };
   ```
3. **Type**
   - Store the type of the entity
   ```c++
   struct Type {
      ecs::EntityType entityType = ecs::EntityType::NOTYPE;

      bool operator==(const Type &other) const {
         return (entityType == other.entityType);
      }
   };
   ```
4. **Life**
   - Store the life of the entity
   ```c++
   struct Life {
      std::size_t nbLives;

      bool operator==(const Life &other) const {
         return (nbLives == other.nbLives);
      }
   };
   ```
5. **Shooting**
   - Defines if the entity is curently shooting
   ```c++
   struct Shooting {
      bool isShooting = false;

      bool operator==(const Shooting &other) const {
         return (isShooting == other.isShooting);
      }
   };
   ```
6. **SpriteInfos**
   - Store all main property of the sprite of the entity<br>
   Like the sprite path, the patern for animation or the scale of the sprite
   ```c++
   struct SpriteInfos {
      std::string spritePath;
      std::size_t patternNb = 1;
      std::size_t firstPattern = 0;
      float scaleFactor = 1.0;
      CustomColor color = { 245, 245, 245, 255 };
      float spriteWidth = 0, spriteHeight = 0;

      bool operator==(const SpriteInfos &other) const {
          return (spritePath == other.spritePath && patternNb == other.patternNb && firstPattern == other.firstPattern &&
                  scaleFactor == other.scaleFactor && color.a == other.color.a && color.r == other.color.r && color.g == other.color.g && color.b == other.color.b && spriteWidth == other.spriteWidth && spriteHeight == other.spriteHeight);
      }
   };
   ```
7. **Drawable**
   - Defines the texture (image) information associated with an entity.
   ```c++
   struct Drawable {
      bool isLoaded = false;
      Texture2D spriteTexture = Texture2D{};
      Rectangle spriteRect = Rectangle{};
   };
   ```
8. **SoundEffectInfos**
   - Store characteristics about a sound
   ```c++
   struct SoundEffectInfos {
      std::string soundPath;
      bool isLoaded = false;
      bool hasToBePlayed = false;

      bool operator==(const SoundEffectInfos &other) const {
         return (soundPath == other.soundPath && isLoaded == other.isLoaded && hasToBePlayed == other.hasToBePlayed);
      }
   };

   ```
   ```
9. **SoundEffectObject**
   - Store a sound to be played
   ```c++
   struct SoundEffectInfos {
      Sound sound = Sound{};
   };
   ```
   ```
10. **EnemySpawn**
   - Handles the logic for spawning enemies.
   ```c++
   struct EnemySpawn {
      float timeBeforeSpawn;
      int waveSize;
      int maxSpawnWidth = Lib::getScreenWidth();
      int minSpawnWidth = 0;
      int maxSpawnHeight = Lib::getScreenHeight();
      int minSpawnHeight = 0;
   };
   ```
11. **ShootingRate**
   - Defines the shooting rate of the entity (work with Shooting)
   ```c++
   struct ShootingRate {
      float shootRate = 0;
   };
   ```
12. **BackgroundMusicInfos**
   - Store informations about the background music of an entity, such as the music path, the volume and if the music has to be played
   ```c++
   struct BackgroundMusicInfos {
      std::string musicPath;
      float volume;
      bool hasToBePlayed = false;
      bool isLoaded = false;
      bool isCurrentlyPlayed = false;

      bool operator==(const BackgroundMusicInfos &other) const {
         return (musicPath == other.musicPath && volume == other.volume && isLoaded == other.isLoaded &&
                  hasToBePlayed == other.hasToBePlayed && isCurrentlyPlayed == other.isCurrentlyPlayed);
      }
   };
   ```
13. **BackgroundMusicObject**
   - Store the music object that has to be played
   ```c++
   struct BackgroundMusicObject {
      Music music = Music{};
   };
   ```
14. **Score**
- Store the score and the incrementation score value of an entity
```c++
   std::size_t scoreIncrementation = 0;
   int score = 0;

   bool operator==(const Score &other) const {
       return (score == other.score && scoreIncrementation == other.scoreIncrementation);
   }
```
15.   **TextInfos**
   - Store informations about the text of an entity
   ```c++
        struct TextInfos {
            std::string textToDisplay;
            int x;
            int y;
            int size;
            std::string fontPath;
            CustomColor color = { 245, 245, 245, 255 };
            bool fontIsLoaded = false;
            bool hasChanged = false;

            bool operator==(const TextInfos &other) const {
                return (textToDisplay == other.textToDisplay && x == other.x && y == other.y && size == other.size &&
                        fontPath == other.fontPath && color.a == other.color.a && color.r == other.color.r && color.g == other.color.g && color.b == other.color.b);
            }
        };
   ```
16.   **TextFont**
   - Store the text font linked to a text of an entity
   ```c++
      struct TextFont {
            Font font = Font{};
      };
   ```

<div id='AddComponents'/>

## How to add a new component
To add a new component you have to create a new structure like the other ones.<br>
There is 2 case when adding a component :
* The component has to be sent between the server and the client
* The component won't be sent between the server and the client

### 1st case the component won't be sent between the server and the client
In this case you just have to build your system and use your component (pretty easy).

### 2nd case the component has to be sent between the server and the client
In this case you have to do multiple things.

First you have to add the line `areComponentsEquals` in the overload operator in `EntityData` structure with your component :
```c++
bool operator==(const EntityData &other) const {
    if (entityID == other.entityID && name == other.name) {
        return (
         areComponentsEquals<ecs::component::Position>(name, ecs::ComponentName::POSITION, component, other.component) ||
         areComponentsEquals<ecs::component::Velocity>(name, ecs::ComponentName::VELOCITY, component, other.component) ||
         areComponentsEquals<ecs::component::BackgroundMusicInfos>(name, ecs::ComponentName::MUSIC, component, other.component));
    }
    return false;
}
```
You also have to add the prototype of `<<` and `>>` at the end of the file.<br>
Now you have to go in Components.cpp and add your overload operator, one for the serialization and one for deserialization.
```c++
void operator<<(std::ostringstream &oss, const ecs::component::BackgroundMusicInfos &music) {
    oss << music.musicPath << ' ' << music.volume << ' ' << music.hasToBePlayed << ' ' << music.isLoaded << ' ' << music.isCurrentlyPlayed;
}
```
```c++
void operator>>(std::istringstream &iss, ecs::component::BackgroundMusicInfos &music) {
    iss >> music.musicPath >> music.volume >> music.hasToBePlayed >> music.isLoaded >> music.isCurrentlyPlayed;
}
```
The variables called in the two operators must be in the same position.<br>
You will also see two bigger overload operator that don't take a component but a EntityData.<br>
In those operators you have to add an if statement with a call of the lib function to serialize or deserialize.
```c++
void operator>>(std::istringstream &iss, EntityData &entityData) {

    iss >> entityData.entityID;
    iss >> entityData.name;
    if (entityData.name == ecs::ComponentName::POSITION)
        network::HandleData::deserializeEntityCompotent<ecs::component::Position>(iss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::VELOCITY)
        network::HandleData::deserializeEntityCompotent<ecs::component::Velocity>(iss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::SPRITEINFOS)
        network::HandleData::deserializeEntityCompotent<ecs::component::SpriteInfos>(iss, entityData.name, entityData.component);
}
```
And
```c++
void operator<<(std::ostringstream &oss, const EntityData &entityData) {

    oss << entityData.entityID << ' ';
    oss << entityData.name << ' ';
    if (entityData.name == ecs::ComponentName::POSITION)
        network::HandleData::serializeEntityCompotent<ecs::component::Position>(oss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::VELOCITY)
        network::HandleData::serializeEntityCompotent<ecs::component::Velocity>(oss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::SPRITEINFOS)
        network::HandleData::serializeEntityCompotent<ecs::component::SpriteInfos>(oss, entityData.name, entityData.component);
    oss << '\n';
}
```

<div id='Systems'/>

## Systems

**Systems** contain the game logic and are responsible for manipulating entities via their components.

### How to add a system
To add a system you have to create a new system that will make the action or modification on the components.

A new system has to have a .hpp and .cpp<br>
In the hpp file you must have those include :
```hpp
#include "Components.hpp"
#include "ISystem.hpp"
```
And the class of your system must be like this one (you can add as many private function as you want):
```hpp
namespace ecs {
    namespace system {
        class "Name of your system" : virtual public ecs::system::ISystem {
          public:
            "Name of your system"() = default;
            ~"Name of your system"() = default;

            void operator()(Registry &reg) override;
        };
    }
}
```
In your cpp file you must have the overload operator, this is a basic template for an overloaded operator :
```c++
void ecs::system::LifeSystem::operator()(Registry &reg) {
    SparseArray<component::ComponentName> &yourComponentArray = reg.getComponents<component::ComponentName>();

    for (std::size_t i = 0; i < yourComponentArray.size(); ++i) {
        if (yourComponentArray[i]) {
            if (yourComponentArray[i]->YourData)
               yourComponentArray[i]->YourDataToModify = NewData;
        }
    }
}
```
The last thing to do is to register the component and the system in the game (see the game documentation).

<div id='Registry'/>

## Registry

The Registry is basicaly where the entities are stored.<br>
The Registry has a lot of function that allow us to do actions on our entites
There is the different command that you can do with the Registry :
   - `entity_t spawnEntity()` : use to create a new entity and return the entity
   - `killEntity(entity_t const &e)` : use to kill an entity
   - `entity_t entityFromIndex(std::size_t idx)` : return an entity by his index in the list of entities
   - `bool entityExist(const entity_t &ent)` : check if an entity exist and return a bool
   - `addPlayer(std::size_t clientId, entity_t entity)` : add a player to a client id
   - `std::size_t getPlayerByClientId(std::size_t clientId)` : use to get a player entity by the ID of a client
   - `std::map<std::size_t, entity_t> getPlayers()` : return the map of all player assoiated to a client
   - `removeComponent(entity_t const &from)` : remove a component that as been registered
   - `SparseArray<Component> &registerComponent()` : register a component
   - `SparseArray<Component> &getComponents()` : get a specific component array that store the component of every entity
   - `addSystem(Function const &f)` : add a system that will be used in the game
   - `runSystems()` : run every system that have been added
   - `addInitPlayerSystem(Function const &f)` : take a system in parameter, initialise a new player
   - `runInitPlayerSystem()` : run the initialisation of the player

<div id='ECSArchitecture'/>

## ECS Architecture

The **ECS architecture** used allows separating the logic of managing entities (in the systems) from the data specific to entities (components). This makes adding new systems and components simple and effective, thus facilitating the extensibility of the engine.
