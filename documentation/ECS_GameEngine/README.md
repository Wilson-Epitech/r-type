
---

# Game Engine Documentation

## Introduction

This project implements a game engine based on the **ECS** (Entity-Component-System) architecture for the **R-Type** game. ECS allows flexible and efficient management of game entities, where each entity is composed of multiple components and is managed by systems that manipulate these components.

## Engine Structure

Our game engine is composed of three main elements in the ECS architecture:
- **Entities**: Represent game objects (players, enemies, projectiles, etc.).
- **Components**: Store the data associated with entities.
- **Systems**: Contain the logic to manipulate entities through their components.

## Components

**Components** are data blocks used to define the characteristics of an entity. Below is an overview of the available components:

1. **Drawable**
   - Defines the texture (image) information associated with an entity.
   - Fields: `isLoaded`, `spriteTexture`, `spriteRect`.

2. **SpriteInfos**
   - Defines additional information for sprites (images), such as the image path, number of patterns, etc.
   - Fields: `spritePath`, `patternNb`, `scaleFactor`.

3. **SoundEffect**
   - Manages sounds associated with an entity, such as sound effects for shooting or explosions.
   - Fields: `soundPath`, `sound`, `isLoaded`, `hasToBePlayed`.

4. **Controllable**
   - Indicates whether an entity can be controlled by the player.
   - Fields: `isControllable`.

5. **Position**
   - Defines the position of an entity in the game space.
   - Fields: `x`, `y`.

6. **Velocity**
   - Defines an entity's velocity, used to handle movement.
   - Fields: `vx`, `vy`, `possibleVelocity`.

7. **Type**
   - Indicates the type of entity (player, enemy, shot, etc.).
   - Fields: `type`.

8. **Life**
   - Manages the remaining lives for an entity.
   - Fields: `nbLives`.

9. **ShootingRate**
   - Manages the shooting rate of entities.
   - Fields: `shootRate`.

10. **EnemySpawn**
    - Handles the logic for spawning enemies.
    - Fields: `timeBeforeSpawn`, `waveSize`.

11. **Background Music**
    - Handles the logic for spawning enemies.
    - Fields: `musicPath`, `volume`.

## Systems

**Systems** contain the game logic and are responsible for manipulating entities via their components.

1. **Collision System**
   - Detects collisions between entities and handles interactions such as reducing lives.
   - Function: `operator()`

2. **Control System**
   - Handles player inputs to move controllable entities (keyboard or gamepad).
   - Function: `operator()`

3. **Draw System**
   - Manages the visual rendering of entities on the screen.
   - Function: `operator()`

4. **Enemy Firing System**
   - Manages the logic for enemy shooting.
   - Function: `operator()`

5. **Enemy Spawn System**
   - Manages the appearance of enemy waves at regular intervals.
   - Function: `operator()`

6. **Firing System**
   - Manages the player's shooting logic.
   - Function: `operator()`

7. **Kill Offscreen Entities System**
   - Removes entities that go off-screen (e.g., projectiles).
   - Function: `operator()`

8. **Life System**
   - Removes entities that no longer have lives.
   - Function: `operator()`

9. **Position System**
   - Updates the position of entities based on their velocity.
   - Function: `operator()`

10. **Sound System**
    - Manages sound playback for entities with sound effects.
    - Function: `operator()`

11. **Sprite Animation System**
    - Handles sprite animation by modifying their texture to create animations.
    - Function: `operator()`

12. **Music System**
    - Manages music for the game.
    - Function: `operator()`

13. **Player Spawn System**
    - Manages when a new player is created.
    - Function: `operator()`

## ECS Architecture

The **ECS architecture** used allows separating the logic of managing entities (in the systems) from the data specific to entities (components). This makes adding new systems and components simple and effective, thus facilitating the extensibility of the engine.
