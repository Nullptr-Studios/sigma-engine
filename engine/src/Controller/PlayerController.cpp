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
    std::string action = m_inputSystem.GetAction();
    if (!action.empty()) {
      PlayAction(action);
      return;
    }
    UpdateMovement();
  }
}

void PlayerController::UpdateMovement() {
  AEVec2 movement = m_inputSystem.GetMovement();

  // Check for movement input
  bool isMovingX = movement.x != 0.f;
  bool isMovingY = movement.y != 0.f;

  if (isMovingX || isMovingY) {
    m_facingDirection = AEVec2(movement.x, movement.y);
  }

  // Acceleration: When the player is moving, increase velocity up to the maximum velocity
  if (isMovingX) {
    // Apply acceleration and clamp velocity
    velocityX += acceleration * movement.x;
    // Ensure velocity doesn't exceed max speed if necessary (optional)
    // velocityX = std::clamp(velocityX, -MAX_VELOCITY, MAX_VELOCITY);
  } else {
    // Deceleration: When there's no movement, decrease velocity
    if (velocityX != 0.f) {
      // Apply deceleration towards zero
      if (velocityX > 0.f) {
        velocityX -= acceleration;
        if (velocityX < 0.f) velocityX = 0.f;  // Clamp to zero if overshooting
      } else if (velocityX < 0.f) {
        velocityX += acceleration;
        if (velocityX > 0.f) velocityX = 0.f;  // Clamp to zero if overshooting
      }
    }
  }

  if (isMovingY) {
    velocityY += acceleration * movement.y;
    // Ensure velocity doesn't exceed max speed if necessary (optional)
    // velocityY = std::clamp(velocityY, -MAX_VELOCITY, MAX_VELOCITY);
  } else {
    // Deceleration: When there's no movement, decrease velocity
    if (velocityY != 0.f) {
      // Apply deceleration towards zero
      if (velocityY > 0.f) {
        velocityY -= acceleration;
        if (velocityY < 0.f) velocityY = 0.f;  // Clamp to zero if overshooting
      } else if (velocityY < 0.f) {
        velocityY += acceleration;
        if (velocityY > 0.f) velocityY = 0.f;  // Clamp to zero if overshooting
      }
    }
  }

  // Update position based on movement direction and velocity
  m_position = {velocityX, velocityY, 0};
  m_character->transform.position += m_position;

  // Transition to MOVING state if velocity is above a small threshold
  if (std::abs(velocityX) > 0.1f || std::abs(velocityY) > 0.1f) {
    m_state = MOVING;
  } else {
    m_state = IDLE; // Transition to IDLE when velocity is 0
  }

  // Debug output (optional)
  // std::cout << "Position: " << m_position.x << ", " << m_position.y
  //           << " VelocityX: " << velocityX << " velocityY: " << velocityY << std::endl;
}


void PlayerController::PlayAction(std::string action) {
  m_state = ACTION;
  std::cout << "Action: " << action << "\n";
  // TODO: Do Animation
  // TODO: After animation return to NEURTRAL
  EndAction(action);
}

void PlayerController::EndAction(std::string action) { m_state = IDLE; }

} // namespace FNFE
