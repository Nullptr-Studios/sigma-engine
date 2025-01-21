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
    pos += m_inputSystem.GetMovement() * .16667;
  }
}
void PlayerController::PlayAction(PlayerAction action) {
  if (m_state != NEUTRAL) {
    std::cout << "WHAT THE FUCK HOW\n";
  }
  std::cout << this << " Start Action\n";
  // TODO: Do Animation
  // TODO: After animation return to NEURTRAL
  std::thread(
      [this, action]()
      {
        std::this_thread::sleep_for(std::chrono::microseconds(900));
        EndAction(action);
      })
      .detach();
}
void PlayerController::EndAction(PlayerAction action) {
  m_state = NEUTRAL;
}

} // namespace FNFE
