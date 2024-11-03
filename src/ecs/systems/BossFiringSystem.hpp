/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** BossFiringSystem
*/

#ifndef BOSSFIRINGSYSTEM_HPP_
#define BOSSFIRINGSYSTEM_HPP_

#include "Components.hpp"
#include "ISystem.hpp"
#include <math.h>

#define SHOT_POS_X 1200 ///< start position of the shoot in x
#define MAX_SHOT_Y 800  ///< maximum starting position of the shoot in y
#define MIN_SHOT_Y 200  ///< minimum starting position of the shoot in y

namespace ecs {
    namespace system {

        /**
         * @class BossFiringSystem
         * @brief Handles the firing of the boss' shot.
         */
        class BossFiringSystem : virtual public ecs::system::ISystem {
          public:
            BossFiringSystem() = default;
            ~BossFiringSystem() = default;

            /**
             * @brief Executes the boss firing logic.
             * @param reg The ECS registry containing entities and components.
             */
            void operator()(Registry &reg) override;

          protected:
          private:
            /**
             * @brief Creates a shot fired by the boss entity.
             * This method generates a new shot entity for a boss when the firing conditions are met.
             * It sets the position, velocity, and other components of the shot.
             * @param index The index of the boss entity firing the shot.
             * @param reg The ECS registry where the shot entity will be created.
             */
            void addShot(const std::size_t &index, Registry &reg);

            /**
             * @brief Finds the index of an boss shot component.
             * This method retrieves the index of the entity that has a shooting component, used to manage boss shots.
             * @param reg The ECS registry containing entities and components.
             * @return std::size_t The index of the boss entity with a shooting component.
             */
            std::size_t getBossShootIndex(Registry &reg);

            /**
             * @brief Finds the index of the chosen entity.
             * This method retrieves the index of the entity passed as parameter.
             * @param types Array of types components for entities.
             * @return std::size_t The index of the chosen entity.
             */
            std::size_t findChosenBossEntity(SparseArray<component::Type> &types, const ecs::EntityType &entityToFind);

            /**
             * @brief Change the velocity of the boss shoot.
             * This method change the velocity of the boss shoot if the boss pattern is 'FOLLOW'
             * The shoot will go in the direction of the player
             * @param reg The ECS registry containing entities and components.
             * @param bossIndex The index of the boss entity firing the shot.
             * @param shootIndex The index of the boss shoot entity.
             * @return float the angle of the shoot.
             */
            float changeShootVelocity(Registry &reg, const std::size_t &bossIndex, const std::size_t &shootIndex);

            /**
             * @brief Finds boss's nearest player.
             * @param reg The ECS registry containing entities and components.
             * @param index The index of the boss entity.
             * @return std::size_t The index of the boss's nearest player.
             */
            std::size_t findClosestPlayer(Registry &reg, const std::size_t &index);

            /**
             * @brief Set the second following shot for the final boss.
             * @param reg The ECS registry containing entities and components.
             * @param types Array of types components for entities.
             * @param index The index of the boss entity.
             */
            void additionalShotFinalLevel(Registry &reg, SparseArray<component::Type> types, const std::size_t &index);

            /**
             * @brief Change the texture used for the level2 boss shot.
             * @param reg The ECS registry containing entities and components.
             * @param types Array of types components for entities.
             */
            void changeShootSprite(Registry &reg, SparseArray<component::Type> types);
        };
    }
}

#endif /* !BOSSFIRINGSYSTEM_HPP_ */
