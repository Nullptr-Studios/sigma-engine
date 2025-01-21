/**
 * @file InputSystem.hpp
 * @author dante
 * @date 1/20/2025
 *
 * @brief Manages player input
 */

#pragma once

namespace FNFE {
/**
 * @brief all the keycodes for the buttons are mapped to this enum
 */
enum PlayerAction {
  NULL_ACTION = 0, ///<@brief No Action
  PLAYER_JUMP = AE_GAMEPAD_B, ///<@brief action jump
  PLAYER_NORMAL = AE_GAMEPAD_A, ///<@brief action normal attack
  PLAYER_SECONDARY = AE_GAMEPAD_X, ///<@brief action secoundary attack
  PLAYER_ULT = AE_GAMEPAD_Y ///<@brief action ult attack
};
/**
 * @brief debug tool for printing what button the player pressed
 * @param action action
 * @return the actions button on the xbox controller
 */
char ToChar(PlayerAction action);

/**
 * @class InputSystem manages inputs and input buffers
 */
class InputSystem {
public:
  /**
   * @brief updates all the buffers with corresponding input from the controller
   * @param controllerId the id of the controller
   */
  void UpdateInput(int controllerId);
  /**
   * @brief returns the next action of the player and sets the buffer to NULL_ACTION
   * @return next action of the player 
   */
  PlayerAction GetAction();
  /**
   * @brief returns current movement for player
   * @return movement for player
   */
  AEVec2 GetMovement() {return m_directionBuffer;};

private:
  /**
   * @brief list of actions that make up a combo
   */
  std::vector<PlayerAction> m_comboBuffer{};
  /**
   * @brief action input buffer
   */
  PlayerAction m_inputBuffer{};
  /**
   * @brief directinal input buffer
   */
  AEVec2 m_directionBuffer{};
  /**
   * @brief timeout buffer for the input buffers
   */
  time_t m_timeBuffer{};
};
} // namespace FNFE
