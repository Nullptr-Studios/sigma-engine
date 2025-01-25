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

#define PLAYER_ACCELERATION 0.1f /// @brief player acceleration
#define PLAYER_MAX_VELOCITY 1000000.0f ///@brief maximum player velocity

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
 * @class PlayerController manages per player input and actions
 */
class PlayerController final : public ControllerComponent {
public:
  PlayerController(Character* character) : ControllerComponent(character) {
    velocity = 0;
  }
  ~PlayerController() override {}
  void Update() override;

private:
  float velocity; ///@brief player velocity variable, 0 by default
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
  void PlayAction(PlayerAction action);
  /**
   * @brief function played when action is ended
   * @param action the action that ended
   */
  void EndAction(PlayerAction action);

  /**
   * @brief id of the controller being used
   */
  //TODO: Maybe I'm dumb but I don't see how u actually get controller id -a
  int m_controllerId{-1};
  /**
   * @brief input system that manages input
   */
  InputSystem m_inputSystem;
  /**
   * @brief what state the player is in
   */
  PlayerState m_state = IDLE;
};
} // namespace FNFE
