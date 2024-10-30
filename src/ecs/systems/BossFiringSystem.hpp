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
                * @brief Creates a shot fired by a boss entity.
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

        };
    }
}

#endif /* !BOSSFIRINGSYSTEM_HPP_ */
