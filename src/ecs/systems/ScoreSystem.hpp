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
            void fillTextInfos(const std::size_t index, Registry &reg);
            std::string removeNumber(std::string str);
            std::size_t getScoreTextIndex(Registry &reg);
        };
    }
}

#endif /* !SCORESYSTEM_HPP_ */
