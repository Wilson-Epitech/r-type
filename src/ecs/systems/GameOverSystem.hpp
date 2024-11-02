/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** GameOverSystem
*/

/**
 * @file GameOverSystem.hpp
 * @brief Declares the system responsible for handling the game-over state in the ECS.
 * This file contains the `GameOverSystem` class, which monitors entities for game-over conditions
 * and manages the final score display and game-over message.
 */
#ifndef GAMEOVERSYSTEM_HPP_
#define GAMEOVERSYSTEM_HPP_

#define GAME_OVER_TEXT "GAME_OVER"
#define FINAL_SCORE_TEXT "Final_Score:"
#define EXIT_PROMPT "Press_[ESC]_to_Exit"
#define FONT_PATH "assets/ARCADE_N.TTF"
#define GAME_OVER_TEXT_SIZE 80
#define SCORE_TEXT_SIZE 40
#define EXIT_TEXT_SIZE 30
#define RED_GAMEOVER                                                                                                                       \
    { 255, 0, 0, 255 } // Red color for game over
#define ORANGE_GAMEOVER                                                                                                                    \
    { 255, 165, 0, 255 } // Orange color for game over
#define GREY_GAMEOVER                                                                                                                      \
    { 211, 211, 211, 255 } // Grey color for game over

#include "Components.hpp"
#include "ISystem.hpp"
#include <string>

namespace ecs {
    namespace system {

        /**
         * @class GameOverSystem
         * @brief System responsible for managing the game-over state and displaying the final score.
         * The `GameOverSystem` checks entities for a game-over condition, and when detected,
         * it displays the game-over message, final score, and handles the end-of-game flow.
         */
        class GameOverSystem : virtual public ecs::system::ISystem {
          public:
            GameOverSystem() = default;
            ~GameOverSystem() = default;

            /**
             * @brief Executes the game-over system, checking for game-over conditions.
             * When the game-over state is triggered, it displays the final score and reason
             * and updates the registry to reflect the end of the game.
             * @param reg The ECS registry containing entities and their components.
             */
            void operator()(Registry &reg) override;

          private:
            /**
             * @brief Checks and displays the game-over screen.
             * This function renders the final score and a game-over message on the screen
             * if the game-over condition is met.
             * @param gameOverComp The `GameOverComponent` containing game-over state, reason and score.
             */
            void displayGameOverScreen(const component::GameOver &gameOverComp, Registry &reg);
        };
    }
}

#endif /* !GAMEOVERSYSTEM_HPP_ */
