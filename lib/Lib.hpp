/*
** R_TYPE PROJECT, 2024
** responcive_window
** File description:
** lib
*/

#ifndef LIB_HPP_
    #define LIB_HPP_

    #include <raylib.h>

    #define SCREEN_WIDTH 1920
    #define SCREEN_HEIGHT 1080

class Lib {
    public:
        Lib() = default;
        ~Lib() = default;

        static Vector2 getScreenRatio();
        static int getScreenHeight();
        static int getScreenWidth();

    private:
};

#endif /* !LIB_HPP_ */
