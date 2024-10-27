/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** RunGame
*/

/**
 * @file RunGame.hpp
 * @brief Declares the `RunGame` class responsible for running the game and managing the game loop.
 * This file contains the `RunGame` class, which sets up the game environment, initializes the window and audio device,
 * and runs the main game loop. It provides a framework for managing game states and processing systems within the ECS.
 */
#ifndef RUNGAME_HPP_
#define RUNGAME_HPP_

#include "Client.hpp"
#include "IGame.hpp"
#include "Lib.hpp"
#include "Server.hpp"
#include <functional>
#include <raylib.h>

#define UPDATE_TIME 0.02 ///< Set the time to update the game.

class RunGame {
  public:
    /**
     * @brief Constructs the game serverside.
     * This constructor sets up the necessary game variables before running the game server loop.
     * It also sets a custom logging level and defines a callback function to manage the game state.
     * @tparam Function The type of the game state function, which defines the current state of the game.
     * @param server The server instance that will manage the game.
     * @param reg The ECS registry that manages entities and components.
     * @param gameName The name of the game, used as the window title.
     * @param gameState A function that manages the game's state transitions.
     */
    template <typename Function>
    RunGame(std::unique_ptr<network::Server> &server, ecs::Registry &reg, Function const &gameState)
        : _server(std::move(server)), _client(nullptr), _reg(reg), _gameState(gameState), _time0(std::chrono::steady_clock::now()),
          _time1(std::chrono::steady_clock::now()) {}
    /**
     * @brief Constructs the game clientside.
     * This constructor sets up the game before running the game client loop. It initializes the window, maximizes it, and initializes the
     * audio device. It also sets a custom logging level and defines a callback function to manage the game state.
     * @tparam Function The type of the game state function, which defines the current state of the game.
     * @param client The client instance that will receive game data from the server.
     * @param reg The ECS registry that manages entities and components.
     * @param gameName The name of the game, used as the window title.
     * @param gameState A function that manages the game's state transitions.
     */
    template <typename Function>
    RunGame(std::unique_ptr<network::Client> &client, ecs::Registry &reg, const std::string &gameName, Function const &gameState)
        : _server(nullptr), _client(std::move(client)), _reg(reg), _gameState(gameState), _time0(std::chrono::steady_clock::now()),
          _time1(std::chrono::steady_clock::now()) {
        SetTraceLogLevel(TraceLogLevel::LOG_FATAL);
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        InitWindow(900, 450, gameName.c_str());
        // InitWindow(Lib::getScreenWidth(), Lib::getScreenHeight(), gameName.c_str());
        // MaximizeWindow();
        InitAudioDevice();
    }

    /**
     * @brief Destructor for the `RunGame` class.
     * This destructor is responsible for cleaning up any resources initialized by the game engine.
     */
    ~RunGame() = default;

    /**
     * @brief Runs the game loop.
     * This method is responsible for running the main game loop, handling input, updating systems, and rendering.
     * It continues running the loop until the game state indicates that the game should end.
     */
    void run();

  private:
    /**
     * @brief Runs the game loop.
     * This method is responsible for checking if the Elapsed Time is bigger than the Update_Time define.
     */
    bool isUpdateTimeElapsed();

    /**
     * @brief Runs the server loop.
     * This method is responsible for running the main server loop, handling clients, updating the state of the ongoing game.
     * It continues running the loop until the server is closed.
     */
    void runServer();

    /**
     * @brief Runs the client loop.
     * This method is responsible for running the main client loop, sending heartbeat, display the game's information given by the server.
     * It continues running the loop until the client is closed.
     */
    void runClient();

    std::unique_ptr<network::Server> _server;                    ///< Unique pointer to an instance of the server
    std::unique_ptr<network::Client> _client;                    ///< Unique Pointer to an instance of the client
    ecs::Registry _reg;                                          ///< The ECS registry managing entities and components.
    std::function<game::GameStatus(ecs::Registry &)> _gameState; ///< Function to manage the current game state.
    std::chrono::steady_clock::time_point _time0;                ///< Base Clock
    std::chrono::steady_clock::time_point _time1;                ///< Clock to compare the base clock to
    std::chrono::milliseconds _timeElapsed;                      ///< The chrono to see the time that passed.
};

#endif /* !RUNGAME_HPP_ */
