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
 * @class PlayerController manages per player input and actions
 */
class PlayerController final : public ControllerComponent {
public:
  PlayerController(Transform *transform) : ControllerComponent(transform) {}
  void Update() override;

private:
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
  int m_controllerId{};
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
