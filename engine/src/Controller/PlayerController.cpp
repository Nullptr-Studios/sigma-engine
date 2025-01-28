#include "PlayerController.hpp"
#include "InputSystem.hpp"
#include "Objects/Character.hpp"
#include "Objects/Object.hpp"
// #include "../../../../src/Player.hpp"

namespace FNFE {

void PlayerController::CheckControllers() {
  for (int i = 0; i<=3; i++) {
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
    if (action == "jump") {
      m_state = AIR;
    }
    if (!action.empty()) {
      PlayAction(action);
      return;
    }
    UpdateMovement();
  }
}

void PlayerController::UpdateMovement() {
  maxVelocity = 100.0f;
  maxAirVelocity = maxVelocity/2;
  AEVec2 movement = m_inputSystem.GetMovement();

  // Check for movement input
  bool isMovingX = movement.x != 0.f;
  bool isMovingY = movement.y != 0.f;
  if (isMovingX || isMovingY) {
    m_facingDirection = AEVec2(movement.x, movement.y);
  }

  if (m_state == AIR) {
    // Acceleration: When the player is moving, increase velocity up to the maximum velocity
    if (isMovingX) {
      // Apply acceleration and clamp velocity
      velocityX = std::clamp(velocityX + inAirAcceleration * movement.x , -maxAirVelocity, maxAirVelocity);
    } else {
      // Deceleration: When there's no movement, decrease velocity
      if (velocityX != 0.f) {
        // Apply deceleration towards zero
        if (velocityX > 0.f) {
          velocityX -= inAirAcceleration;
          if (velocityX < 0.f) velocityX = 0.f;  // Clamp to zero if overshooting
        } else if (velocityX < 0.f) {
          velocityX += inAirAcceleration;
          if (velocityX > 0.f) velocityX = 0.f;  // Clamp to zero if overshooting
        }
      }
    }

    // Actual jumping shit:
    if (jumpPeaked) {
        if (m_position.y > m_position.z) {
          velocityY -= playerGravity;
        } else {
          m_character->transform.position.y = m_character->transform.position.z;
          jumpPeaked = false;
          velocityY = 0.f;
          velocityX = 0.f;
          velocityZ = 0.f;
          m_state = IDLE;
          return;
        }
      } else {
        if (m_character->transform.position.y - m_character->transform.position.z >= jumpHeight ) {
          jumpPeaked = true;
        } else {
          velocityY = std::clamp(velocityY + inAirAcceleration, 0.0f, maxAirVelocity);
        }
    }
      m_position = {static_cast<float>(velocityX * AEGetFrameTime()),
      static_cast<float>(velocityY * AEGetFrameTime()),
      0.0f};
      m_character->transform.position += m_position;
      return;

  } else {

    // Acceleration: When the player is moving, increase velocity up to the maximum velocity
    if (isMovingX) {
      // Apply acceleration and clamp velocity
      velocityX = std::clamp(velocityX + acceleration * movement.x , -maxVelocity, maxVelocity);
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
      velocityY = std::clamp(velocityY + acceleration * movement.y, -maxVelocity, maxVelocity);
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
    m_position = {static_cast<float>(velocityX * AEGetFrameTime()),
      static_cast<float>(velocityY * AEGetFrameTime()),
      static_cast<float>(velocityY * AEGetFrameTime())};
    m_character->transform.position += m_position;

    // Transition to MOVING state if velocity is above a small threshold
    if (std::abs(velocityX) > 0.1f || std::abs(velocityY) > 0.1f) {
      m_state = MOVING;

      //DEPRECATED ANIMATIONS
      /*m_character->m_animComp->PlayAnim();

      if (velocityX > 0.f) {
        m_character->m_animComp->SetCurrentAnim("ToastWalkingRight");
      }else if (!velocityY > 0.f) {
        m_character->m_animComp->SetCurrentAnim("ToastWalkingLeft");
      }

      if (velocityY > 0.f) {
        m_character->m_animComp->SetCurrentAnim("ToastWalkingUp");
      }else if (!velocityX > 0.f) {
        m_character->m_animComp->SetCurrentAnim("ToastWalkingDown");
      }*/

      if (velocityX > 0.f || velocityY > 0.f) {}
    } else {
      m_state = IDLE; // Transition to IDLE when velocity is 0
      //m_character->m_animComp->StopAnim();
    }
  }
}

void PlayerController::PlayAction(std::string action) {
  if (m_state == AIR) {
    m_state = ACTION;
    // TODO: Do AIR Animation
    // After animation, return to NEUTRAL
    m_state = AIR;
  } else {
    m_state = ACTION;
    std::cout << "Action: " << action << "\n";
    // TODO: Do Animation
    // After animation return to NEURTRAL

    velocityX = 0.0f;
    velocityY = 0.0f;
    EndAction(action);
  }
}

void PlayerController::EndAction(std::string action) { m_state = IDLE; }

} // namespace FNFE