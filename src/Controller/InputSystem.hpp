/**
 * @file InputSystem.hpp
 * @author dante
 * @date 1/20/2025
 *
 * @brief Manages player input
 */

#pragma once

namespace FNFE {
enum PlayerAction {
  NULL_ACTION = 0,
  PLAYER_JUMP = AE_GAMEPAD_A,
  PLAYER_NORMAL = AE_GAMEPAD_X,
  PLAYER_SECONDARY = AE_GAMEPAD_B,
  PLAYER_ULT = AE_GAMEPAD_Y
};
class InputSystem {
public:
  void UpdateInput(int controllerId);
  PlayerAction GetAction();
  AEVec2 GetMovement() {return m_directionBuffer;};

private:
  std::vector<PlayerAction> m_comboBuffer{};
  PlayerAction m_inputBuffer{};
  AEVec2 m_directionBuffer{};
  time_t m_timeBuffer{};
};
} // namespace FNFE
void testPlayer();
