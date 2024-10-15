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
#include <raylib.h>

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
             * @brief Fills the sprite sizes for all entities that have sprite information.
             * This method ensures that each entity with a `SpriteInfos` component also has a
             * corresponding `SpriteSize` component filled with the appropriate dimensions.
             * @param spritesInfos Array of sprite information components.
             * @param spritesSizes Array of sprite size components.
             */
            void fillSpriteSize(SparseArray<component::SpriteInfos> &spritesInfos, SparseArray<component::SpriteSize> &spritesSizes);

            /**
             * @brief Computes the hitbox of a given entity based on its position and sprite size.
             * @param positions Array of position components for entities.
             * @param spritesSizes Array of sprite size components for entities.
             * @param index The index of the entity in the sparse arrays.
             * @return Rectangle The computed hitbox of the entity.
             */
            Rectangle spriteHitbox(SparseArray<component::Position> &positions, SparseArray<component::SpriteSize> &spritesSizes,
                                   std::size_t index);

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
        };
    }
}

#endif /* !COLLISIONSYSTEM_HPP_ */
