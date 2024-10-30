/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** CollisionSystem
*/

/**
 * @file CollisionSystem.hpp
 * @brief Declares the collision management system for the game engine ECS.
 * This file contains the `CollisionSystem` class that handles collision detection between
 * entities in the game, such as players, shots, and enemies.
 */

#ifndef COLLISIONSYSTEM_HPP_
#define COLLISIONSYSTEM_HPP_

#include "Components.hpp"
#include "ISystem.hpp"
#include <fstream>

#if _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif

#define BYTES_OFFSET 16   ///< offset in bytes for png files to start reading width and height for sprites
#define LENGTH_IN_BYTES 4 ///< length in bytes for width and height for png files

namespace ecs {
    namespace system {

        /**
         * @class CollisionSystem
         * @brief Handles collision detection and resolution between entities.
         */
        class CollisionSystem : virtual public ecs::system::ISystem {
          public:
            CollisionSystem() = default;
            ~CollisionSystem() = default;

            /**
             * @brief Executes the collision detection logic.
             * @param reg The ECS registry containing entities and components.
             */

            void operator()(Registry &reg) override;

          private:
            /**
             * @brief Retrive the sprites sizes by reading the width and the height in the .png file.
             * @param spritesInfos Array of sprite informations components for entities.
             */
            void retrieveSpritesSizes(SparseArray<component::SpriteInfos> &spritesInfos);

            /**
             * @brief Computes the hitbox of a given entity based on its position and sprite size.
             * @param positions Array of position components for entities.
             * @param spritesInfos Array of sprite informations components for entities.
             * @param index The index of the entity in the sparse arrays.
             * @return CustomRectangle The computed hitbox of the entity.
             */
            ecs::CustomRectangle spriteHitbox(SparseArray<component::Position> &positions,
                                              SparseArray<component::SpriteInfos> &spritesInfos, std::size_t index);

            /**
             * @brief Check if the two given entities in parameter are in collision or not.
             * @param CustomRectangle hitbox of the first entity.
             * @param CustomRectangle hitbox of the second entity.
             * @return true If entities are in collision
             * @ return false Otherwise.
             */
            bool areEntitiesInCollision(ecs::CustomRectangle firstEntity, ecs::CustomRectangle secondEntity);

            /**
             * @brief Determines if two entities are enemies.
             * This method checks if two entities, based on their types, should be considered enemies.
             * @param firstType The type of the first entity.
             * @param secondType The type of the second entity.
             * @return true If the entities are enemies.
             * @return false Otherwise.
             */
            bool areEntitiesEnemies(const ecs::EntityType &firstType, const ecs::EntityType &secondType);

            /**
             * @brief Checks if the collision is between a player and a player shot.
             * @param firstType The type of the first entity.
             * @param secondType The type of the second entity.
             * @return true If it's a collision between a player and their shot.
             * @return false Otherwise.
             */
            bool isPlayerAndPlayerShot(const ecs::EntityType &firstType, const ecs::EntityType &secondType);

            /**
             * @brief Checks if the collision is between an enemy and an enemy shot.
             * @param firstType The type of the first entity.
             * @param secondType The type of the second entity.
             * @return true If it's a collision between an enemy and their shot.
             * @return false Otherwise.
             */
            bool isEnemyAndEnemyShot(const ecs::EntityType &firstType, const ecs::EntityType &secondType);

            /**
             * @brief Checks if the collision is between a player shot and an enemy shot.
             * @param firstType The type of the first entity.
             * @param secondType The type of the second entity.
             * @return true If it's a collision between a player shot and an enemy shot.
             * @return false Otherwise.
             */
            bool isPlayerShotAndEnemyShot(const ecs::EntityType &firstType, const ecs::EntityType &secondType);

            /**
             * @brief Checks if either of the entities involved in the collision is a background entity.
             * @param firstType The type of the first entity.
             * @param secondType The type of the second entity.
             * @return true If one of the entities is a background.
             * @return false Otherwise.
             */
            bool isBackground(const ecs::EntityType &firstType, const ecs::EntityType &secondType);

            /**
             * @brief Checks if the collision is between an enemy and a Powerup.
             * @param firstType The type of the first entity.
             * @param secondType The type of the second entity.
             * @return true If it's a collision between an enemy and a Powerup.
             * @return false Otherwise.
             */
            bool isEnemyAndPowerup(const ecs::EntityType &firstType, const ecs::EntityType &secondType);

            /**
             * @brief Checks if the collision is between an enemy shot and a Powerup.
             * @param firstType The type of the first entity.
             * @param secondType The type of the second entity.
             * @return true If it's a collision between an enemy shot and a Powerup.
             * @return false Otherwise.
             */
            bool isEnemyShotAndPowerup(const ecs::EntityType &firstType, const ecs::EntityType &secondType);

            /**
             * @brief Checks if the collision is between a player shot and a Powerup.
             * @param firstType The type of the first entity.
             * @param secondType The type of the second entity.
             * @return true If it's a collision between a player shot and a Powerup.
             * @return false Otherwise.
             */
            bool isPlayerShotAndPowerup(const ecs::EntityType &firstType, const ecs::EntityType &secondType);

            /**
             * @brief give one pv to the player when he hit a powerup and store the index of the player that hit a powerup.
             * @param reg The ECS registry containing entities and components.
             * @param i The index of the first entity.
             * @param j The index of the second entity.
             */
            void powerUpLives(Registry &reg, const std::size_t &i, const std::size_t &j);

            /**
             * @brief Checks if the collision is between the level 1 boss and a level1 boss' shot.
             * @param firstType The type of the first entity.
             * @param secondType The type of the second entity.
             * @return true If it's a collision between the Level1 boss and a Level1 boss shot.
             * @return false Otherwise.
             */
            bool isBossLevel1AndBossLevel1Shot(const ecs::EntityType &firstType, const ecs::EntityType &secondType);

            /**
             * @brief Checks if the collision is between the level 1 boss and an enemy shot.
             * @param firstType The type of the first entity.
             * @param secondType The type of the second entity.
             * @return true If it's a collision between the Level1 boss and an enemy shot.
             * @return false Otherwise.
             */
            bool isBossLevel1AndEnemyShot(const ecs::EntityType &firstType, const ecs::EntityType &secondType);

            /**
             * @brief Checks if the collision is between the level 1 boss and an enemy.
             * @param firstType The type of the first entity.
             * @param secondType The type of the second entity.
             * @return true If it's a collision between the Level1 boss and an enemy.
             * @return false Otherwise.
             */
            bool isBossLevel1AndEnemy(const ecs::EntityType &firstType, const ecs::EntityType &secondType);

            /**
             * @brief Checks if the collision is between the level 1 boss' shot and a powerup.
             * @param firstType The type of the first entity.
             * @param secondType The type of the second entity.
             * @return true If it's a collision between the Level1 boss' shot and a powerup.
             * @return false Otherwise.
             */
            bool isBossLevel1ShotAndPowerUp(const ecs::EntityType &firstType, const ecs::EntityType &secondType);

            /**
             * @brief Checks if the collision is between the level 1 boss' shot and a player's shot.
             * @param firstType The type of the first entity.
             * @param secondType The type of the second entity.
             * @return true If it's a collision between the Level1 boss' shot and a player's shot.
             * @return false Otherwise.
             */
            bool isBossLevel1ShotAndPlayerShot(const ecs::EntityType &firstType, const ecs::EntityType &secondType);
        };
    }
}

#endif /* !COLLISIONSYSTEM_HPP_ */
