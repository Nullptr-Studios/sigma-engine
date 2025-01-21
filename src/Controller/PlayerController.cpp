#include "PlayerController.hpp"
#include <thread>
#include "InputSystem.hpp"
namespace FNFE {

void PlayerController::Update() {
  m_inputSystem.UpdateInput(0);
  if (m_state == NEUTRAL) {
    PlayerAction action = m_inputSystem.GetAction();
    if (action != NULL_ACTION) {
      PlayAction(action);
      return;
    }
    pos += m_inputSystem.GetMovement() * 10;
  }
}
void PlayerController::PlayAction(PlayerAction action) {
  m_state = ACTION;
  // TODO: Do Animation
  // TODO: After animation return to NEURTRAL
  EndAction(action);
}
void PlayerController::EndAction(PlayerAction action) { m_state = NEUTRAL; }

} // namespace FNFE
