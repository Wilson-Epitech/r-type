/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** IGame
*/

#ifndef IGAME_HPP_
#define IGAME_HPP_

#include "Registry.hpp"

namespace game {
    enum GameStatus { PLAYING, WON, LOOSE };

    class IGame {
      public:
        IGame() = default;
        ~IGame() = default;

        virtual GameStatus operator()(ecs::Registry &reg) = 0;
        virtual GameStatus gameState(ecs::Registry &reg) = 0;
        virtual void initializeGame() = 0;
        virtual void spawnEntities() = 0;
        virtual void registerComponents() = 0;
        virtual void addComponentsToEntities() = 0;
        virtual void addSystemsToRegister() = 0;
        virtual ecs::Registry &getRegister() = 0;
        virtual std::string &getGameName() = 0;
    };
}

#endif /* !IGAME_HPP_ */
