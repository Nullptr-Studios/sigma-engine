#include "Collider.hpp"
#include <Objects/Actor.hpp>

namespace sigma::Collision {

void BoxCollider::DebugDraw(Actor* parent, unsigned color = AE_COLORS_GREEN) const {
  glm::vec2 size = glm::vec2(fabs(box.left) + fabs(box.right), fabs(box.top) + fabs(box.bottom));
  glm::vec2 pos  = parent->transform.position;
  AEGfxRect(pos.x, pos.y, 0.0f, size.x, size.y, color);
}

} // namespace sigma
