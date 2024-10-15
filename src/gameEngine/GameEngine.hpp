/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** GameEngine
*/

/**
 * @file GameEngine.hpp
 * @brief Declares the `GameEngine` class responsible for running the game engine and managing the game loop.
 * This file contains the `GameEngine` class, which sets up the game environment, initializes the window and audio device,
 * and runs the main game loop. It provides a framework for managing game states and processing systems within the ECS.
 */

#ifndef GAMEENGINE_HPP_
#define GAMEENGINE_HPP_

#include "Components.hpp"
#include "IGame.hpp"
#include "Lib.hpp"
#include "Registry.hpp"
#include <functional>
#include <raylib.h>

#define MARGIN_TOP 10  ///< Top margin for UI elements or FPS display.
#define FPS_MARGIN 100 ///< Margin for the FPS display.
#define FPS_LIMIT 60   ///< Limit for the game's frame rate.

class GameEngine {
  public:
    /**
     * @brief Constructs the `GameEngine` and initializes the window and audio device.
     * This constructor sets up the game engine by initializing the window, maximizing it, and initializing the audio device.
     * It also sets a custom logging level and defines a callback function to manage the game state.
     * @tparam Function The type of the game state function, which defines the current state of the game.
     * @param reg The ECS registry that manages entities and components.
     * @param gameName The name of the game, used as the window title.
     * @param gameState A function that manages the game's state transitions.
     */
    template <typename Function>
    GameEngine(ecs::Registry &reg, const std::string &gameName, Function const &gameState) : _reg(reg), _gameState(gameState) {
        SetTraceLogLevel(TraceLogLevel::LOG_FATAL);
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        InitWindow(Lib::getScreenWidth(), Lib::getScreenHeight(), gameName.c_str());
        MaximizeWindow();
        InitAudioDevice();
    }

    /**
     * @brief Destructor for the `GameEngine` class.
     * This destructor is responsible for cleaning up any resources initialized by the game engine.
     */
    ~GameEngine() = default;

    /**
     * @brief Runs the game loop.
     * This method is responsible for running the main game loop, handling input, updating systems, and rendering.
     * It continues running the loop until the game state indicates that the game should end.
     */
    void run();

  private:
    ecs::Registry _reg;                                          ///< The ECS registry managing entities and components.
    std::function<game::GameStatus(ecs::Registry &)> _gameState; ///< Function to manage the current game state.
};

#endif /* !GAMEENGINE_HPP_ */
