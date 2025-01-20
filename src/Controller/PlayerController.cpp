#include "PlayerController.hpp"
#include "InputSystem.hpp"

namespace FNFE {

void PlayerController::Update() {
  m_inputSystem.UpdateInput(0);
  if (state == NEUTRAL) {
    PlayerAction action = m_inputSystem.GetAction();
    if (action != NULL_ACTION) {
      PlayAction(action);
      return;
    }
    pos += m_inputSystem.GetMovement()*.16667;
  }
}
void PlayerController::PlayAction(PlayerAction action) {
  state = ACTION;
  std::cout << action << std::endl;
  // TODO: Do Animation
  // TODO: After animation return to NEURTRAL
  state = NEUTRAL;
}
} // namespace FNFE
