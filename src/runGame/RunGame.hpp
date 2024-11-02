/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** RunGame
*/

#ifndef RUNGAME_HPP_
#define RUNGAME_HPP_

#include "Components.hpp"
#include "DLLoader.hpp"
#include "IGame.hpp"
#include "INetwork.hpp"
#include <functional>

#define WIN32_LEAN_AND_MEAN
#define NOGDI

#include <winsock2.h>
#include <windows.h>

#undef Rectangle
#undef CloseWindow
#undef ShowCursor
#undef LoadImage
#undef DrawText
#undef DrawTextEx

#include <raylib.h>
#define UPDATE_TIME 0.02 ///< Set the time to update the game.

#if __linux__
#define SERVER_LIB_PATH "sharedLibraries/libserver.so" ///< Path to the server shared library for linux
#define CLIENT_LIB_PATH "sharedLibraries/libclient.so" ///< Path to the client shared library for linux
#elif __APPLE__
#define SERVER_LIB_PATH "sharedLibraries/libserver.dylib" ///< Path to the server shared library for macOS
#define CLIENT_LIB_PATH "sharedLibraries/libclient.dylib" ///< Path to the client shared library for macOS
#elif _WIN32
#define SERVER_LIB_PATH "sharedLibraries/server.dll" ///< Path to the server shared library for Windows
#define CLIENT_LIB_PATH "sharedLibraries/client.dll" ///< Path to the client shared library for Windows
#endif

/**
 * @class RunGame
 * @brief Class responsible for running the game and managing the game loop.
 * It sets up the game environment, initializes the window and audio device, and runs the main game loop.
 * It provides a framework for managing game states and processing systems within the ECS.
 */

class RunGame {
  public:
    /**
     * @brief Constructs the game serverside.
     * This constructor sets up the necessary game variables before running the game server loop, such as the server instance that is loaded
     * dynamically It also sets a custom logging level and defines a callback function to manage the game state.
     * @tparam Function The type of the game state function, which defines the current state of the game.
     * @param reg The ECS registry that manages entities and components.
     * @param gameState A function that manages the game's state transitions.
     * @param port The port on which the server listens.
     * @param pool A thread pool to manage concurrent tasks.
     */

    template <typename Function>
    RunGame(ecs::Registry &reg, Function const &gameState, const std::size_t &port, asio::thread_pool &pool)
        : _reg(reg), _gameState(gameState), _time0(std::chrono::steady_clock::now()), _time1(std::chrono::steady_clock::now()) {
        _networkLoader = std::make_unique<DLLoader<network::INetwork>>(SERVER_LIB_PATH);
        _networkInstance = _networkLoader->getInstance(port, pool);
    }
    /**
     * @brief Constructs the game clientside.
     * This constructor sets up the game before running the game client loop, such as the client instance that is loaded dynamically. It
     * initializes the window, maximizes it, and initializes the audio device. It also sets a custom logging level and defines a callback
     * function to manage the game state.
     * @tparam Function The type of the game state function, which defines the current state of the game.
     * @param client The client instance that will receive game data from the server.
     * @param reg The ECS registry that manages entities and components.
     * @param gameName The name of the game, used as the window title.
     * @param gameState A function that manages the game's state transitions.
     * @param host The server's hostname or IP address.
     * @param port The server's port number.
     */
    template <typename Function>
    RunGame(ecs::Registry &reg, const std::string &gameName, Function const &gameState, const std::string &host, const std::string &port)
        : _reg(reg), _gameState(gameState), _time0(std::chrono::steady_clock::now()), _time1(std::chrono::steady_clock::now()) {
        _networkLoader = std::make_unique<DLLoader<network::INetwork>>(CLIENT_LIB_PATH);
        _networkInstance = _networkLoader->getInstance(host, port);
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
     * @brief Runs the game loop, based on the network instance type.
     * This method is responsible for running the main game loop, handling input, updating systems, and rendering.
     * It continues running the loop until the game state indicates that the game should end.
     */
    void run();

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

  private:
    /**
     * @brief Runs the game loop.
     * This method is responsible for checking if the Elapsed Time is bigger than the Update_Time define.
     */
    bool isUpdateTimeElapsed();

    std::unique_ptr<DLLoader<network::INetwork>> _networkLoader; ///< Unique pointer to a DLLoader network instance
    std::unique_ptr<network::INetwork> _networkInstance;         ///< Unique pointer to a network instance
    ecs::Registry _reg;                                          ///< The ECS registry managing entities and components.
    std::function<game::GameStatus(ecs::Registry &)> _gameState; ///< Function to manage the current game state.
    std::chrono::steady_clock::time_point _time0;                ///< Base Clock
    std::chrono::steady_clock::time_point _time1;                ///< Clock to compare the base clock to
    std::chrono::milliseconds _timeElapsed;                      ///< The chrono to see the time that passed.
};

#endif /* !RUNGAME_HPP_ */
