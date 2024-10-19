/*
** R_TYPE PROJECT, 2024
** branch
** File description:
** ScoreSystem
*/

#ifndef SCORESYSTEM_HPP_
#define SCORESYSTEM_HPP_

#include "Components.hpp"
#include "ISystem.hpp"

namespace ecs {
    namespace system {
        class ScoreSystem : virtual public ecs::system::ISystem {
          public:
            ScoreSystem() = default;
            ~ScoreSystem() = default;

            void operator()(Registry &reg) override;

          private:
            /**
             * @brief Change the text with the score of the entity.
             * @param index Array of drawable components for entities.
             * @param reg The ECS Registry.
             */
            void fillTextInfos(const std::size_t index, Registry &reg);
            /**
             * @brief Remove the last number of a string.
             * @param str Array of drawable components for entities.
             */
            std::string removeNumber(std::string str);
            /**
             * @brief Return the score of the team.
             * @param reg Array of drawable components for entities.
             */
            std::size_t getScoreTextIndex(Registry &reg);
        };
    }
}

#endif /* !SCORESYSTEM_HPP_ */
