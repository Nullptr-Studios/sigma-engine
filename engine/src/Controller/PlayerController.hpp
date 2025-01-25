/**
 * @file PlayerController.hpp
 * @author alexey
 * @date 1/22/2025
 *
 * @brief Manages and interprets input to action
 * TODO: ALEXEY DO THESE FILES PlayerController and ControllerComponent
 */
#pragma once
#include "ControllerComponent.hpp"
#include "InputSystem.hpp"
namespace FNFE {
class InputSystem;
}
namespace FNFE {


/**
 * @enum PlayerState for the current state the player is in
 */
enum PlayerState {
  IDLE,   ///< @brief The player is standing still
  MOVING, ///< @brief The player is moving
  AIR,    ///< @brief The player is in the air
  ACTION, ///< @brief The player is preforming an action
};

/**
 * @class PlayerController
 * @brief Manages per player input and actions
 */
class PlayerController final : public ControllerComponent {
public:
  PlayerController(Character* character) : ControllerComponent(character), m_inputSystem("assets/core/keybinds.json") {}
  ~PlayerController() override {}
  void Update() override; ///< @brief Call this every frame to update the player position
  /**
   * @brief Get the direction the player is facing
   * This is like the direction the player is moving, but it never returns a zero value
   * @return The direction the player last moved
   */
  AEVec2 GetFacingDirection() const { return m_facingDirection; }
  
private:
  float acceleration = 0.1f; /// @brief player acceleration
  float maxVelocity = 10.0f; ///@brief max player acceleration
  float velocityX = 0; ///@brief player velocity variable, 0 by default
  float velocityY = 0; ///@brief player velocity variable, 0 by default

  /**
   * @brief checks if any gamepad is connected, sets controller id to gamepad id
   */
  //TODO: make this work with multiple controllers
void CheckControllers();
  /**
   * @brief helper function to update movement
   * @param movement_action specified movement action
   */
  void UpdateMovement();
  //TODO: have a combo buffer to keep track of current combo
  /**
   * @brief plays specifed action
   * @param action specifed action
   */
  void PlayAction(std::string action);
  /**
   * @brief function played when action is ended
   * @param action the action that ended
   */
  void EndAction(std::string action);

  /**
   * @brief id of the controller being used
   */
  int m_controllerId = -1;
  /**
   * @brief input system that manages input
   */
  InputSystem m_inputSystem;
  /**
   * @brief what state the player is in
   */
  PlayerState m_state = IDLE;
  
  AEVec2 m_facingDirection = AEVec2(1.0f, 0.0f); ///< @brief Direction the player last moved
  
};

}
