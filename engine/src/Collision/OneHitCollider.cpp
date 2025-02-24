//
// Created by Dario on 12/02/2025.
//

#include "OneHitCollider.hpp"
#include "ColisionVisualizer.hpp"
#include "Factory.hpp"

namespace Sigma::Collision {

void OneHitCollider::Do(const glm::vec3 &position, const glm::vec3 &size, float damage,glm::vec2 knockback, Sigma::Object *owner, bool debugDraw) {
  transform.position = position;
  if (debugDraw) {
    //auto d = GET_FACTORY->CreateObject<ColisionVisualizer>("DebugCollider", position, size, 1.0f);
    //d->SetTexture("assets/core/debug_yellow.png");
  }

  // Create collider
  m_collider->box.Set(size);
  m_collider->damage = damage;
  m_collider->knockback = knockback;
  m_collider->SetOwner(owner);
  m_collider->enabled = true;

  SetStartHandled(false);
}

void OneHitCollider::Init() {
  Object::Init();
  m_collider = std::make_unique<Collision::BoxCollider>(Collision::PLAYER | Collision::ENEMY, Collision::DAMAGE);
}

void OneHitCollider::Update(double deltaTime) {
  Object::Update(deltaTime);
  if (m_collider->enabled) {
    m_collider->enabled = false;
  }
}

}
