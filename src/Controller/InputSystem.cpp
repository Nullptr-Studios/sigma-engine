#include "InputSystem.hpp"
namespace FNFE {


#define CLAMP_DIR(val) ((val > .5) ? 1 : (val < -.5) ? -1 : 0)
#define IS_TRIGGERED(G_CODE,K_CODE) (controllerId <0)?AEInputKeyTriggered(K_CODE):AEInputGamepadButtonTriggered(controllerId, G_CODE)
void InputSystem::UpdateInput(int controllerId) {
  if (controllerId >=0 ) {
    m_directionBuffer = AEInputGamepadStickLeft(controllerId);
  } else {
    m_directionBuffer = {};
    m_directionBuffer.x -= AEInputKeyPressed(K_LEFT);
    m_directionBuffer.x += AEInputKeyPressed(K_RIGHT);
    m_directionBuffer.y -= AEInputKeyPressed(K_DOWN);
    m_directionBuffer.y += AEInputKeyPressed(K_UP);
  }
  m_directionBuffer.x = CLAMP_DIR(m_directionBuffer.x);
  m_directionBuffer.y = CLAMP_DIR(m_directionBuffer.y);
  AEVector2Normalize(&m_directionBuffer, &m_directionBuffer);
  if (IS_TRIGGERED(G_NORMAL, K_NORMAL)) {
    m_inputBuffer = PLAYER_NORMAL;
    m_timeBuffer = time(nullptr);
  } else if (IS_TRIGGERED(G_SECONDARY,K_SECONDARY)) {
    m_inputBuffer = PLAYER_SECONDARY;
    m_timeBuffer = time(nullptr);
  } else if (IS_TRIGGERED(G_JUMP,K_JUMP)) {
    m_inputBuffer = PLAYER_JUMP;
    m_timeBuffer = time(nullptr);
  } else if (IS_TRIGGERED(G_ULT,K_ULT)) {
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
