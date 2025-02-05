#include "Character.hpp"

namespace Sigma {

Character::~Character() {}

void Character::Init() {
  Actor::Init();

  m_animComp = std::make_unique<Animation::AnimationComponent>();
}


glm::mat3 &Character::GetTextureTransform() {
  if (m_animComp == nullptr) {
    m_tMtx = glm::mat3(1.0f);
    return m_tMtx;
  }
  return m_animComp->GetTextureMatrix();
}

#pragma region MovementSystem

void Character::Move(glm::vec2 direction) {
  if (!isJumping) {
    velocity += direction * maxSpeed;
    velocity = glm::clamp(velocity, -maxSpeed, maxSpeed);
  }else {
    velocity.x += direction.x * maxSpeed;
    velocity.x = glm::clamp(velocity.x, -maxSpeed, maxSpeed);
  }
}

void Character::Jump() {
  if (!isJumping) {
    velocity.y = jumpVel * AEGetFrameRate();
    isJumping = true;
    m_movementYFloor = transform.position.y;
  }
}

void Character::UpdateMovement(double delta)
{
  // Apply gravity
  if (isJumping) {
    velocity.y += gravity * delta;
    velocity.y = glm::clamp(velocity.y, -terminalVel, terminalVel);
  }

  // Apply deceleration when no input is given in X axis
  if (std::abs(velocity.x) > 0.01f) {
    if (velocity.x > 0) {
      velocity.x -= friction * delta;
      glm::max(velocity.x, 0.0f); 
    }
    else
    {
      velocity.x += friction * delta;
      glm::min(velocity.x, 0.0f); 
    }
  }
  
  // Apply deceleration when no input is given in Y axis
  if (!isJumping) {
    if (std::abs(velocity.y) > 0.01f) {
      if (velocity.y > 0) {
        velocity.y -= friction * delta;
        glm::max(velocity.y, 0.0f); 
      }
      else
      {
        velocity.y += friction * delta;
        glm::min(velocity.y, 0.0f); 
      }
    }
  }

  // Update position
  transform.position.x += velocity.x * delta;
  transform.position.y += velocity.y * delta;

  // Ground collision 
  if (isJumping && transform.position.y <= m_movementYFloor) {
    transform.position.y = m_movementYFloor;
    velocity.y = 0;
    isJumping = false;
  }

  
  //Update Z
  if (!isJumping)
    transform.position.z = -transform.position.y;
}
#pragma endregion

} // namespace FNFE
