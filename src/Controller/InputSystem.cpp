#include "InputSystem.hpp"
namespace FNFE {

/**
 * @def CHECK_INPUT(G_CODE, K_CODE)
 * @brief Checks whether a specific key or gamepad button is triggered.
 *      based on what the controllerId is id < 0 for keyboard
 *      0-3 for gamepad
 * @param G_CODE gamepad code for the gamepad button
 * @param K_CODE keyboard key for the keyboard
 *
 * @note this macro will only work in InputSystem::UpdateInput for its
 *      reliance on the controllerId variable
 */
#define CHECK_INPUT(G_CODE,K_CODE) (controllerId <0)?AEInputKeyTriggered(K_CODE):AEInputGamepadButtonTriggered(controllerId, G_CODE)
void InputSystem::UpdateInput(int controllerId) {
  // Use gamepad stick or keyboard keys for player movement
  if (controllerId >=0 ) {
    m_directionBuffer = AEInputGamepadStickLeft(controllerId);
  } else {
    m_directionBuffer = {};
    m_directionBuffer.x -= AEInputKeyPressed(K_LEFT);
    m_directionBuffer.x += AEInputKeyPressed(K_RIGHT);
    m_directionBuffer.y -= AEInputKeyPressed(K_DOWN);
    m_directionBuffer.y += AEInputKeyPressed(K_UP);
  }
  AEVector2Normalize(&m_directionBuffer, &m_directionBuffer);
  // Use Gamepad buttons vs keyboard keys for player actions
  if (CHECK_INPUT(G_NORMAL, K_NORMAL)) {
    m_inputBuffer = PLAYER_NORMAL;
    m_timeBuffer = time(nullptr);
  } else if (CHECK_INPUT(G_SECONDARY,K_SECONDARY)) {
    m_inputBuffer = PLAYER_SECONDARY;
    m_timeBuffer = time(nullptr);
  } else if (CHECK_INPUT(G_JUMP,K_JUMP)) {
    m_inputBuffer = PLAYER_JUMP;
    m_timeBuffer = time(nullptr);
  } else if (CHECK_INPUT(G_ULT,K_ULT)) {
    m_inputBuffer = PLAYER_ULT;
    m_timeBuffer = time(nullptr);
  } else {
    if (m_inputBuffer != NULL_ACTION && (time(nullptr), m_timeBuffer) > 2) {
      m_inputBuffer = NULL_ACTION;
      std::cout << "InputBuffer Timeout\n";
    }
  }
}

PlayerAction InputSystem::GetAction() {
  PlayerAction tmp = m_inputBuffer;
  m_inputBuffer = NULL_ACTION;
  return tmp;
}

char ToChar(PlayerAction action) {
  switch (action) {
    case PLAYER_NORMAL:
      return 'A';
    case PLAYER_SECONDARY:
      return 'X';
    case PLAYER_JUMP:
      return 'B';
    case PLAYER_ULT:
      return 'Y';
    default:
      return '?';
  }
}
} // namespace FNFE
