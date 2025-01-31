/**
 * @file InputSystem.hpp
 * @author dante
 * @date 1/20/2025
 *
 * @brief Manages player input
 */

#pragma once
#include <glm/vec2.hpp>
#include <string>

namespace FNFE {
/**
 *
 * @param button char button representing xbox gamepad button
 * @return ae_gamepad_<key> of the button input
 */
int ToGamepadKey(char button);

/**
 * @class InputSystem manages inputs and input buffers
 */
class InputSystem {
public:
  InputSystem(const std::string &keybindPath);

  /**
   * @brief updates all the buffers with corresponding input from the controller
   * @param controllerId the id of the controller -1 for keyboard
   */
  void UpdateInput(int controllerId);

  /**
   * @brief updates the direction buffer usally for movement all
   * values will be from [-1,1] for x and y values
   * @param controllerId the id of the controller -1 for keyboard
   */
  void UpdateDirection(int controllerId);

  /**
   * @brief updates the player actions
   * @param controllerId the id of the controller -1 for the keyboard
   */
  void UpdateActions(int controllerId);

  /**
   * @brief returns the next action of the player and sets the buffer to NULL_ACTION
   * @return next action of the player
   */
  std::string GetAction();

  /**
   * @brief returns current movement for player
   * @return movement for player
   */
  glm::vec2 GetMovement() { return m_movementBuffer; };

private:
  std::unordered_map<std::string, std::string> m_keyboardActions{};
  std::unordered_map<std::string, std::string> m_keyboardMovement{};
  std::unordered_map<std::string, std::string> m_gamepadActions{};
  std::string m_inputBuffer{}; ///<@brief action input buffer
  glm::vec2 m_movementBuffer{}; ///<@brief directinal input buffer
  int m_movementStick{};
  time_t m_timeBuffer{}; ///<@brief timeout buffer for the input buffers
};

} // namespace FNFE
