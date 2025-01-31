#include "PlayerController.hpp"
#include "InputSystem.hpp"
#include "Objects/Character.hpp"
#include "Objects/Object.hpp"
// #include "../../../../src/Player.hpp"

namespace sigma {

/*
TODO:
  Make this more performant!
  IK you can just call this at object creation but if you do that you can't hotswap it :( -a
 */
void PlayerController::CheckControllers() {
  for (int i = 0; i <= 3; i++) {
    if (AEInputGamepadConnected(i)) {
      m_controllerId = i;
      return;
    }
  }
  m_controllerId = -1;
}

// i guess we will never know what to do -x
void PlayerController::Update() {
  CheckControllers();
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
  maxVelocity = 100.0f;

  glm::vec2 movement = m_inputSystem.GetMovement();

  // Check for movement input
  bool isMovingX = movement.x != 0.f;
  bool isMovingY = movement.y != 0.f;

  if (isMovingX || isMovingY) {
    m_facingDirection = glm::vec2(movement.x, movement.y);
  }

  // Acceleration: When the player is moving, increase velocity up to the maximum velocity
  if (isMovingX) {
    // Apply acceleration and clamp velocity
    velocityX = std::clamp(velocityX + acceleration * movement.x, -maxVelocity, maxVelocity);
    // Ensure velocity doesn't exceed max speed if necessary (optional)
    // velocityX = std::clamp(velocityX, -MAX_VELOCITY, MAX_VELOCITY);
  } else {
    // Deceleration: When there's no movement, decrease velocity
    if (velocityX != 0.f) {
      // Apply deceleration towards zero
      if (velocityX > 0.f) {
        velocityX -= acceleration;
        if (velocityX < 0.f)
          velocityX = 0.f; // Clamp to zero if overshooting
      } else if (velocityX < 0.f) {
        velocityX += acceleration;
        if (velocityX > 0.f)
          velocityX = 0.f; // Clamp to zero if overshooting
      }
    }
  }

  if (isMovingY) {
    velocityY = std::clamp(velocityY + acceleration * movement.y, -maxVelocity, maxVelocity);
    // Ensure velocity doesn't exceed max speed if necessary (optional)
    // velocityY = std::clamp(velocityY, -MAX_VELOCITY, MAX_VELOCITY);
  } else {
    // Deceleration: When there's no movement, decrease velocity
    if (velocityY != 0.f) {
      // Apply deceleration towards zero
      if (velocityY > 0.f) {
        velocityY -= acceleration;
        if (velocityY < 0.f)
          velocityY = 0.f; // Clamp to zero if overshooting
      } else if (velocityY < 0.f) {
        velocityY += acceleration;
        if (velocityY > 0.f)
          velocityY = 0.f; // Clamp to zero if overshooting
      }
    }
  }

  // Update position based on movement direction and velocity
  m_position = {(float) (velocityX * AEGetFrameTime()), (float) (velocityY * AEGetFrameTime()), 0};
  m_character->transform.position += m_position;

  // Transition to MOVING state if velocity is above a small threshold
  if (std::abs(velocityX) > 0.1f || std::abs(velocityY) > 0.1f) {
    m_state = MOVING;

    m_character->m_animComp->PlayAnim();

    if (velocityX > 0.f) {
      m_character->m_animComp->SetCurrentAnim("ToastWalkingRight");
    } else {
      m_character->m_animComp->SetCurrentAnim("ToastWalkingLeft");
    }

    if (velocityY > 0.f) {
      m_character->m_animComp->SetCurrentAnim("ToastWalkingUp");
    } else {
      m_character->m_animComp->SetCurrentAnim("ToastWalkingDown");
    }

    if (velocityX > 0.f) {
    }
  } else {
    m_state = IDLE; // Transition to IDLE when velocity is 0
    m_character->m_animComp->StopAnim();
  }
}

void PlayerController::PlayAction(const std::string &action) {
  m_state = ACTION;
  std::cout << "Action: " << action << "\n";
  // TODO: Do Animation
  // TODO: After animation return to NEURTRAL
  EndAction(action);
}

void PlayerController::EndAction(const std::string &action) { m_state = IDLE; }

} // namespace sigma
