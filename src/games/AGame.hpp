/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** AGame
*/

#ifndef AGAME_HPP_
#define AGAME_HPP_

#include "Components.hpp"
#include "IGame.hpp"

namespace game {
    class AGame : virtual public game::IGame {
      public:
        AGame(const std::string &gameName) : _gameName(gameName) {};
        ~AGame() = default;

        GameStatus operator()(ecs::Registry &reg) override {
            return gameState(reg);
        }

      protected:
        void initializeGame() override {
            spawnEntities();
            registerComponents();
            addComponentsToEntities();
            addSystemsToRegistry();
        };

        ecs::Registry _reg;
        std::string _gameName;
        std::unordered_map<std::string, entity_t> _stringToEntity;
    };
}

#endif /* !AGAME_HPP_ */
