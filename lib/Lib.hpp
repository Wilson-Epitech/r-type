/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** lib
*/

/**
 * @file Lib.hpp
 * @brief Declares utility functions for the R-Type game engine.
 * This file defines a set of helper functions and constants related to screen management, and time conversion.
 */

#ifndef LIB_HPP_
#define LIB_HPP_

#include "CustomTypes.hpp"
#include <chrono>

#define SCREEN_WIDTH 1920  ///< The default screen width for the game window
#define SCREEN_HEIGHT 1080 ///< The default screen height for the game window

#define TO_MILLISECONDS 1000 ///< Conversion factor from seconds to milliseconds

/**
 * @class Lib
 * @brief Utility class providing static functions for game engine operations.
 * Contains helper methods for screen management and time conversion.
 */
class Lib {
  public:
    Lib() = delete;
    ~Lib() = delete;

    /**
     * @brief Retrieves the default screen height.
     * @return The default screen height.
     */
    static int getScreenHeight();

    /**
     * @brief Retrieves the default screen width.
     * @return The default screen width.
     */
    static int getScreenWidth();

    /**
     * @brief Checks whether a position is within the visible screen boundaries.
     * @param pos The position vector to check.
     * @return true if the position is within the screen bounds, false otherwise.
     */
    static bool isOnScreen(ecs::CustomVector2D pos);

    /**
     * @brief Converts time from seconds to milliseconds.
     * @param time Time in seconds.
     * @return Time in milliseconds.
     */
    static double toMilliseconds(double time);

    /**
     * @brief Check if a certain time is past.
     * @param time Time is the time to compare with actual time.
     * @param duration Time in seconds.
     * @return True if the time between the variable time and now is superior or equal to the duration, false otherwise.
     */
    static bool isTimeElapsed(const std::chrono::steady_clock::time_point &time, const float &duration);
};

#endif /* !LIB_HPP_ */
