/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** ControlSystem
*/

/**
 * @file ControlSystem.hpp
 * @brief Declares the control system for player-controlled entities in the game engine ECS.
 * This file contains the `ControlSystem` class, which manages player input through keyboard and gamepad,
 * and applies the corresponding movement to player-controlled entities.
 */

#ifndef CONTROLSYSTEM_HPP_
#define CONTROLSYSTEM_HPP_

#include "Components.hpp"
#include "ISystem.hpp"

#define JOYSTICK_SENSIBILITY 0.6 ///< Sensitivity threshold for joystick movement.

namespace ecs {
    namespace system {

        /**
         * @class ControlSystem
         * @brief Handles player input and applies movement to player-controlled entities.
         * The `ControlSystem` processes input from both the keyboard and gamepad. It adjusts the velocity
         * of the controllable entities based on user inputs, such as moving the entity or shooting.
         */
        class ControlSystem : virtual public ecs::system::ISystem {
          public:
            ControlSystem() = default;
            ~ControlSystem() = default;

            /**
             * @brief Executes the control system, processing input and updating entity movement.
             * This function retrieves the velocity and control components of entities and updates
             * their movement based on keyboard or gamepad input.
             * @param reg The ECS registry containing entities and their components.
             */
            void operator()(Registry &reg) override;

          private:
            /**
             * @brief Handles movement controls using a gamepad.
             * This method adjusts the velocity of the entity based on joystick or button inputs
             * from a gamepad. It processes the movement in the X and Y axes.
             * @param gamepad The gamepad ID.
             * @param velocities Array of velocity components for entities.
             * @param i The index of the entity being controlled.
             */
            void handleController(int &gamepad, SparseArray<component::Velocity> &velocities, const std::size_t &i);

            /**
             * @brief Checks if any trigger button on the gamepad is pressed.
             * This method detects if one of the gamepad's trigger buttons (left or right) is being pressed.
             * @param gamepad The gamepad ID.
             * @return true If a trigger button is pressed.
             * @return false If no trigger button is pressed.
             */
            bool IsGamepadTriggerButtonDown(int &gamepad);
        };
    }
}

#endif /* !CONTROLSYSTEM_HPP_ */
