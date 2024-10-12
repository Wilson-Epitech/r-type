/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** RunGame
*/

#ifndef RUNGAME_HPP_
#define RUNGAME_HPP_

#include "Client.hpp"
#include "Components.hpp"
#include "IGame.hpp"
#include "Registry.hpp"
#include "Server.hpp"
#include <functional>
#include <raylib.h>

#define UPDATE_TIME 0.02

class RunGame {
  public:
    template <typename Function>
    RunGame(std::unique_ptr<network::Server> &server, ecs::Registry &reg, Function const &gameState)
        : _server(std::move(server)), _client(nullptr), _reg(reg), _gameState(gameState), _time0(std::chrono::steady_clock::now()),
          _time1(std::chrono::steady_clock::now()) {
        SetTraceLogLevel(TraceLogLevel::LOG_FATAL);
    }

    template <typename Function>
    RunGame(std::unique_ptr<network::Client> &client, ecs::Registry &reg, const std::string &gameName, Function const &gameState)
        : _server(nullptr), _client(std::move(client)), _reg(reg), _gameState(gameState), _time0(std::chrono::steady_clock::now()),
          _time1(std::chrono::steady_clock::now()) {
        SetTraceLogLevel(TraceLogLevel::LOG_FATAL);
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        InitWindow(Lib::getScreenWidth(), Lib::getScreenHeight(), gameName.c_str());
        MaximizeWindow();
        InitAudioDevice();
    }

    ~RunGame() = default;

    void run();

  private:
    bool isUpdateTimeElapsed();
    void runServer();
    void runClient();

    std::unique_ptr<network::Server> _server;
    std::unique_ptr<network::Client> _client;
    ecs::Registry _reg;
    std::function<game::GameStatus(ecs::Registry &)> _gameState;
    std::chrono::steady_clock::time_point _time0;
    std::chrono::steady_clock::time_point _time1;
    std::chrono::milliseconds _timeElapsed;
};

#endif /* !RUNGAME_HPP_ */
