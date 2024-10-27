/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** CustomTypes
*/

/**
 * @file CustomTypes.hpp
 * @brief Defines custom types used in the ECS system for the game engine, such as custom color and rectangle.
 */

#ifndef CUSTOMTYPES_HPP_
#define CUSTOMTYPES_HPP_

namespace ecs {
    /**
     * @struct CustomVector2D
     * @brief Custom Structure to handle 2D vectors.
     */
    struct CustomVector2D {
        float x = 0;      ///< x value
        float y = 0;      ///< y value
    };

    /**
     * @struct CustomColor
     * @brief Custom Structure to handle colors.
     */
    struct CustomColor {
        unsigned char r = 0; ///< red value
        unsigned char g = 0; ///< green value
        unsigned char b = 0; ///< blue value
        unsigned char a = 0; ///< alpha value

        bool operator==(const CustomColor &other) const {
            return (r == other.r && g == other.g && b == other.b && a == other.a);
        }
    };

    /**
     * @struct CustomRectangle
     * @brief Custom Structure to handle rectangles.
     */
    struct CustomRectangle {
        float x = 0;      ///< X coordinate
        float y = 0;      ///< y coordinate
        float width = 0;  ///< width value
        float height = 0; ///< height value
    };

}

#endif /* !CUSTOMTYPES_HPP_ */