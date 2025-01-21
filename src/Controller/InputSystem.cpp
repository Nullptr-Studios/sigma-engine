#include "InputSystem.hpp"
#include "PlayerController.hpp"
namespace FNFE {

#define CLAMP_DIR(val) (val > .5)?1:(val <-.5)?-1:0
void InputSystem::UpdateInput(int controllerId) {
  m_directionBuffer = AEInputGamepadStickLeft(controllerId);
  m_directionBuffer.x = CLAMP_DIR(m_directionBuffer.x);
  m_directionBuffer.y = CLAMP_DIR(m_directionBuffer.y);
  AEVector2Normalize(&m_directionBuffer, &m_directionBuffer);
  if (AEInputGamepadButtonTriggered(controllerId, PLAYER_NORMAL)) {
    m_inputBuffer = PLAYER_NORMAL;
    m_timeBuffer = time(nullptr);
  } else if (AEInputGamepadButtonTriggered(controllerId, PLAYER_SECONDARY)) {
    m_inputBuffer = PLAYER_SECONDARY;
    m_timeBuffer = time(nullptr);
  } else if (AEInputGamepadButtonTriggered(controllerId, PLAYER_JUMP)) {
    m_inputBuffer = PLAYER_JUMP;
    m_timeBuffer = time(nullptr);
  } else if (AEInputGamepadButtonTriggered(controllerId, PLAYER_ULT)) {
    m_inputBuffer = PLAYER_ULT;
    m_timeBuffer = time(nullptr);
  } else {
    if (m_inputBuffer != NULL_ACTION && (time(nullptr), m_timeBuffer) > 2) {
      m_inputBuffer = NULL_ACTION;
      std::cout << "Ran Out Of Time\n";
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