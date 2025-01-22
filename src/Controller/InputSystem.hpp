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
 * @enum InputBinding K_ for keyboard specific for player one only G_ for gamepad keybinds
 */
enum InputBinding { //TODO: Xein Tasks dante to serialize this into a json
  K_UP = 'W',
  K_LEFT = 'A',
  K_DOWN = 'S',
  K_RIGHT = 'D',
  K_JUMP = ' ',
  K_NORMAL = 'J',
  K_SECONDARY = 'K',
  K_ULT = 'L',
  G_JUMP = AE_GAMEPAD_B,
  G_NORMAL = AE_GAMEPAD_A,
  G_SECONDARY = AE_GAMEPAD_X,
  G_ULT = AE_GAMEPAD_Y,
};
/**
 * @brief all the possible player actions
 */
enum PlayerAction { //TODO: Searialize this also
  NULL_ACTION = NULL, ///<@brief No Action
  PLAYER_NORMAL = AE_GAMEPAD_A, ///<@brief action normal attack
  PLAYER_JUMP = AE_GAMEPAD_B, ///<@brief action jump
  PLAYER_SECONDARY = AE_GAMEPAD_X, ///<@brief action secondary attack
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
  PlayerAction m_inputBuffer{}; ///<@brief action input buffer
  AEVec2 m_directionBuffer{}; ///<@brief directinal input buffer
  time_t m_timeBuffer{}; ///<@brief timeout buffer for the input buffers
};
} // namespace FNFE
