#include "PlayerController.hpp"
#include "InputSystem.hpp"
#include "Objects/Object.hpp"
namespace FNFE {

void PlayerController::Update() {
  m_inputSystem.UpdateInput(0);
  if (m_state == NEUTRAL) {
    PlayerAction action = m_inputSystem.GetAction();
    if (action != NULL_ACTION) {
      PlayAction(action);
      return;
    }
    AEVec2 movement = m_inputSystem.GetMovement();
    m_position += {movement.x,movement.y,0} * 10;
    m_transform->position = m_position;
  }
}
void PlayerController::PlayAction(PlayerAction action) {
  m_state = ACTION;
  std::cout << "Action: " <<ToChar(action) << "\n";
  // TODO: Do Animation
  // TODO: After animation return to NEURTRAL
  EndAction(action);
}
void PlayerController::EndAction(PlayerAction action) { m_state = NEUTRAL; }

} // namespace FNFE
