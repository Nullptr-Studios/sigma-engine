#include "PlayerController.hpp"
#include "InputSystem.hpp"
#include "Objects/Object.hpp"
namespace FNFE {
// TODO: what i have now is just for testing purpose if you want delete it all and redo it
void PlayerController::Update() {
  m_inputSystem.UpdateInput(-1);
  if (m_state == IDLE) {
    PlayerAction action = m_inputSystem.GetAction();
    if (action != NULL_ACTION) {
      PlayAction(action);
      return;
    }
    AEVec2 movement = m_inputSystem.GetMovement();
    m_position += {movement.x,movement.y,0};
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
void PlayerController::EndAction(PlayerAction action) { m_state = IDLE; }

} // namespace FNFE
