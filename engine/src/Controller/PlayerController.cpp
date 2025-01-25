#include "PlayerController.hpp"
#include "InputSystem.hpp"
#include "Objects/Character.hpp"
#include "Objects/Object.hpp"
namespace FNFE {
/*
TODO:

 */
void PlayerController::Update() {
  m_inputSystem.UpdateInput(m_controllerId);
  if (m_state != ACTION) {
    PlayerAction action = m_inputSystem.GetAction();
    if (action != NULL_ACTION) {
      PlayAction(action);
      return;
    }
   UpdateMovement();
  }
}

void PlayerController::UpdateMovement() {
  AEVec2 movement = m_inputSystem.GetMovement();

  // Check for movement input
  bool isMoving = movement.Length() > 0.001f;

  // Acceleration: When the player is moving, increase velocity up to the maximum velocity
  if (isMoving) {
    // Apply acceleration and clamp velocity
    velocity += 0.1f;
  } else {
    // Deceleration: When there's no movement, decrease velocity
    if (velocity > 0.0f) {
      velocity -= PLAYER_ACCELERATION;
      if (velocity < 0.0f) {
        velocity = 0.0f; // Prevent going below zero
      }
    }
  }

  // Update position based on movement direction and velocity
  m_position = {movement.x * velocity, movement.y * velocity, 0};
  m_character->transform.position += m_position;

  // Transition to MOVING state if velocity is above a small threshold
  if (velocity > 0.0f) {
    m_state = MOVING;
  } else {
    m_state = IDLE; // Transition to IDLE when velocity is 0
  }

  // Debug output (optional)
  std::cout << "Position: " << m_position.x << ", " << m_position.y << " Velocity: " << velocity << std::endl;
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
