/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** lib
*/

#ifndef LIB_HPP_
#define LIB_HPP_

#include <vector>
#include <raylib.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#define TO_MILLISECONDS 1000

class Lib {
  public:
    Lib() = delete;
    ~Lib() = delete;

    static Vector2 getScreenRatio();
    static int getScreenHeight();
    static int getScreenWidth();
    static bool isOnScreen(Vector2 pos);
    static double toMilliseconds(double time);
};

#endif /* !LIB_HPP_ */
