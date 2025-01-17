/**
 * @file ControllerComponent.hpp
 * @author dante
 * @date 1/17/2025
 *
 * @brief Abstract Class for player and AI controller
 */
#pragma once

struct AEVec2;

/**
 * @struct ControllerInput all the desired actions of a entitiy in a frame
 */
struct ControllerInput {
  /**
   * @brief direction entity desires to move in
   * x-> left & right
   * y-> jump
   * z-> forward & back
   */
  AEVec3 direction{};
  /**
   * @brief action that the entity will do
   */
  int action{};
};

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
  ControllerInput m_input{};

  void UpdateInput();
  ControllerInput GetInput();
};
