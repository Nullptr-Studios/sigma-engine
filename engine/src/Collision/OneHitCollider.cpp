//
// Created by Dario on 12/02/2025.
//

#include "OneHitCollider.hpp"

#include "ColisionVisualizer.hpp"
#include "Factory.hpp"
void Sigma::Collision::OneHitCollider::Init() {
  Object::Init();

  transform.position = m_position;

  if (m_debug) 
    auto d = GET_FACTORY->CreateObject<ColisionVisualizer>("DebugCollider", m_position, m_size, 1.0f);
  
  

  // Create collider
  m_collider = std::make_unique<Collision::BoxCollider>(Collision::PLAYER | Collision::ENEMY, Collision::DAMAGE);
  m_collider->box.Set(m_size);
  m_collider->damage = m_damage;
  m_collider->SetOwner(m_owner);
  m_collider->enabled = true;

  
}
void Sigma::Collision::OneHitCollider::Update(double deltaTime) {
  Object::Update(deltaTime);
  if (m_collider->enabled) {
    m_collider->enabled = false;
    GET_FACTORY->DestroyObject(this);
  }
}

