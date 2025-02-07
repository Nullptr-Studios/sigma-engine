#include "Character.hpp"

#include "GameManager.hpp"
#include "GameScene.hpp"
#include "Polygon.hpp"

namespace Sigma {

Character::~Character() {}

void Character::Init() {
  Actor::Init();

  m_animComp = std::make_unique<Animation::AnimationComponent>(this);
  GameScene *scene = dynamic_cast<GameScene *>(GET_SCENE);
  if (scene == nullptr) {
    std::cout << GetName() << " failed to get GameScene\n";
    return;
  }
  m_sceneBoundsPoly = dynamic_cast<GameScene *>(GET_SCENE)->GetSceneBoundsPoly();
}


glm::mat3 *Character::GetTextureTransform() {
  /*if (m_animComp == nullptr) {
    m_tMtx = glm::mat3(1.0f);
    return m_tMtx;
  }*/
  // Actor::GetTextureTransform();
  auto mtx = m_animComp->GetTextureMatrix();
  m_tMtx = glm::FromAEX(mtx);
  return &m_tMtx;
}

#pragma region MovementSystem

void Character::Move(glm::vec2 direction) {

  if (!isJumping) {
    velocity.x += direction.x * (accelerationRate * AEGetFrameRate());
    velocity.y += direction.y * (accelerationRate * AEGetFrameRate());

    // Clamp the speed while maintaining direction
    float speed = glm::length(velocity);
    if (speed > maxSpeed)
      velocity = glm::normalize(velocity) * maxSpeed;
  } else {
    velocity.x += direction.x * (accelerationRate * AEGetFrameRate());
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

void Character::UpdateMovement(double delta) {
  // Apply gravity
  if (isJumping) {
    velocity.y += gravity * delta;
    velocity.y = glm::clamp(velocity.y, -terminalVel, terminalVel);
  }

  // Apply deceleration when no input is given in X axis
  if (std::abs(velocity.x) > 0.01f) {
    if (velocity.x > 0) {
      velocity.x -= friction * delta;
      if (velocity.x < 0)
        velocity.x = 0;
      // glm::max(velocity.x, 0.0f);
    } else {
      velocity.x += friction * delta;
      if (velocity.x > 0)
        velocity.x = 0;
      // glm::min(velocity.x, 0.0f);
    }
  }

  // Apply deceleration when no input is given in Y axis
  if (!isJumping) {
    if (std::abs(velocity.y) > 0.01f) {
      if (velocity.y > 0) {
        velocity.y -= friction * delta;
        if (velocity.y < 0)
          velocity.y = 0;
        // glm::max(velocity.y, 0.0f);
      } else {
        velocity.y += friction * delta;
        if (velocity.y > 0)
          velocity.y = 0;
        // glm::min(velocity.y, 0.0f);
      }
    }
  }

  // Calculate if in bounds
  if (m_sceneBoundsPoly != nullptr)
  {
    glm::vec2 newPos = !isJumping ? transform.position : glm::vec2(transform.position.x, m_movementYFloor); 

    newPos.x += velocity.x * delta;
    if (!m_sceneBoundsPoly->isPointInside(newPos)) {
      velocity.x = 0.0f;
    }
    
    if (!isJumping) {
      newPos = transform.position; 

      newPos.y += velocity.y * delta;
      if (!m_sceneBoundsPoly->isPointInside(newPos)) {
        velocity.y = 0.0f;
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

  // Update Z
  if (!isJumping)
    transform.position.z = -transform.position.y;
  
}
#pragma endregion

} // namespace Sigma
