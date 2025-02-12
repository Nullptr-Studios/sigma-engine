//
// Created by Dario on 12/02/2025.
//

#pragma once

#include "Objects/Object.hpp"

namespace Sigma::Collision {

class OneHitCollider final : public Sigma::Object {
public:

  OneHitCollider(uint32_t id, const glm::vec3 &position, const glm::vec3 &size, float damage, Sigma::Object *owner, bool debugDraw = false)
    : Object(id), m_position(position), m_size(size), m_damage(damage), m_owner(owner), m_debug(debugDraw) {}
  
  ~OneHitCollider() override = default;
  
  void Init() override;
  void Update(double deltaTime) override;

private:
  glm::vec3 m_position;
  glm::vec3 m_size;
  float m_damage;

  bool m_debug;

  Object* m_owner;
};


}
