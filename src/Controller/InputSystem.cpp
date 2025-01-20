#include "InputSystem.hpp"

#include "PlayerController.hpp"
namespace FNFE {

#define CLAMP_DIR(val) (val > .5)?1:(val <-.5)?-1:0
void InputSystem::UpdateInput(int controllerId) {
  m_directionBuffer = AEInputGamepadStickLeft(controllerId);
  m_directionBuffer.x = CLAMP_DIR(m_directionBuffer.x);
  m_directionBuffer.y = CLAMP_DIR(m_directionBuffer.y);
  AEVector2Normalize(&m_directionBuffer, &m_directionBuffer);
  if (AEInputGamepadButtonPressed(controllerId, PLAYER_NORMAL)) {
    m_inputBuffer = PLAYER_NORMAL;
  } else if (AEInputGamepadButtonPressed(controllerId, PLAYER_SECONDARY)) {
    m_inputBuffer = PLAYER_SECONDARY;
  } else if (AEInputGamepadButtonPressed(controllerId, PLAYER_JUMP)) {
    m_inputBuffer = PLAYER_JUMP;
  } else if (AEInputGamepadButtonPressed(controllerId, PLAYER_ULT)) {
    m_inputBuffer = PLAYER_ULT;
  }

  
}
PlayerAction InputSystem::GetAction() {
  PlayerAction tmp = m_inputBuffer;
  m_inputBuffer = NULL_ACTION;
  return tmp;
}
} // namespace FNFE

FNFE::PlayerController control{};
void testPlayer() {
  control.Update();
  AEGfxCircle(control.pos.x,control.pos.y, .5f, AE_COLORS_RED);
}
