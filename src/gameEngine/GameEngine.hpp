/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** GameEngine
*/

#ifndef GAMEENGINE_HPP_
#define GAMEENGINE_HPP_

#include "Components.hpp"
#include "IGame.hpp"
#include "Lib.hpp"
#include "Registry.hpp"
#include <functional>
#include <raylib.h>

#define MARGIN_TOP 10
#define FPS_MARGIN 100
#define FPS_LIMIT 60

class GameEngine {
  public:
    template <typename Function>
    GameEngine(ecs::Registry &reg, const std::string &gameName, Function const &gameState) : _reg(reg), _gameState(gameState) {
        SetTraceLogLevel(TraceLogLevel::LOG_FATAL);
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        InitWindow(Lib::getScreenWidth(), Lib::getScreenHeight(), gameName.c_str());
        MaximizeWindow();
        InitAudioDevice();
    }
    ~GameEngine() = default;

    void run();

  private:
    ecs::Registry _reg;
    std::function<game::GameStatus(ecs::Registry &)> _gameState;
};

#endif /* !GAMEENGINE_HPP_ */
