/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** RTypeGame
*/

#ifndef RTYPEGAMECLIENT_HPP_
#define RTYPEGAMECLIENT_HPP_

#include "AGame.hpp"
#include "ControlSystem.hpp"
#include "DrawSystem.hpp"
#include "FiringSystem.hpp"
#include "PositionSystem.hpp"
#include "SpriteAnimationSystem.hpp"

namespace game {
    class RtypeGameClient : virtual public game::AGame {
      public:
        RtypeGameClient(const std::string gameName = "R-Type");
        ~RtypeGameClient() = default;

      private:
        GameStatus gameState(ecs::Registry &reg) override;
        void spawnEntities() override;
        void registerComponents() override;
        void addComponentsToEntities() override;
        void addSystemsToRegistry() override;
    };
}

#endif /* !RTYPEGAMECLIENT_HPP_ */
