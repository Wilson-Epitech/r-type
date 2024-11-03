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

#define GAME_OVER_TEXT "GAME_OVER"        ///< Text displayed when the game is over.
#define FINAL_SCORE_TEXT "Final_Score:"   ///< Text label for displaying the final score.
#define EXIT_PROMPT "Press_[ESC]_to_Exit" ///< Text prompt instructing the player to press [ESC] to exit.
#define FONT_PATH "assets/ARCADE_N.TTF"   ///< File path for the font used in the game-over screen.
#define GAME_OVER_TEXT_SIZE 80            ///< Font size for the "Game Over" text.
#define WIDTH_GAME_OVER 3                 ///< Width divisor for centering the "Game Over" text on the screen.
#define HEIGHT_GAME_OVER 3                ///< Height divisor for positioning the "Game Over" text on the screen.
#define WIDTH_SCORE_TEXT 0.1              ///< Width multiplier for positioning the score text on the screen.
#define HEIGHT_SCORE_TEXT 0.85            ///< Height multiplier for positioning the score text on the screen.
#define WIDTH_EXIT_TEXT 0.6               ///< Width multiplier for positioning the exit text on the screen.
#define HEIGHT_EXIT_TEXT 0.85             ///< Height multiplier for positioning the exit text on the screen.
#define SCORE_TEXT_SIZE 30                ///< Font size for the score text.
#define EXIT_TEXT_SIZE 30                 ///< Font size for the exit prompt text.
#define RED_GAMEOVER                                                                                                                       \
    { 255, 0, 0, 255 } ///< Color definition for the "Game Over" text (red).
#define ORANGE_GAMEOVER                                                                                                                    \
    { 255, 165, 0, 255 } ///< Color definition for the final score text (orange).
#define GREY_GAMEOVER                                                                                                                      \
    { 211, 211, 211, 255 } ///< Color definition for the exit prompt text (grey).

#include "Components.hpp"
#include "ISystem.hpp"

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
             * @param reg Reference to the ECS registry containing entities and their components.
             */
            void displayGameOverScreen(Registry &reg);
        };
    }
}

#endif /* !GAMEOVERSYSTEM_HPP_ */
