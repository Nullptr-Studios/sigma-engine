//
// Created by Dario on 12/02/2025.
//

#pragma once

#include "Objects/Object.hpp"

namespace Sigma::Collision {

class OneHitCollider final : public Sigma::Object {
public:
  explicit OneHitCollider(const id_t id) : Object(id) {}
 
  ~OneHitCollider() override = default;

  void Do(const glm::vec3 &position, const glm::vec3 &size, float damage,glm::vec2 knockback, Object *owner, bool debugDraw = false);

  void Init() override;
  void Update(double deltaTime) override;
};


}
