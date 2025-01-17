/**
 * @file ControllerComponent.hpp
 * @author dante
 * @date 1/17/2025
 *
 * @brief Abstract Class for player and AI controller
 */
#pragma once

// ReSharper disable once CppDoxygenUnresolvedReference
/**
 * @class ControllerComponent Abstract Controller class for PlayerController and AIController to implement
 */
class ControllerComponent {
public:
  ControllerComponent(const ControllerComponent &) = delete;
  ControllerComponent &operator=(const ControllerComponent &) = delete;
  ControllerComponent(ControllerComponent &&) = default;
  ControllerComponent &operator=(ControllerComponent &&) = default;
protected:
};
