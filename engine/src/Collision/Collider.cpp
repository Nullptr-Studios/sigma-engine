#include "Collider.hpp"
#include <Objects/Actor.hpp>
#include "aecore/AEGraphics.h"

namespace Sigma::Collision {

// I think this should work but it's untested -x
// Thanks alpha for having AEGfxRectMinMax -x
void BoxCollider::DebugDraw(Actor* debug, Actor *parent) const {
  debug->transform.scale = box.GetScale();
  auto position = glm::vec2(parent->transform.position.x + box.GetOffset().x, parent->transform.position.y + box.GetOffset().y);
  debug->transform.position = glm::vec3(position.x, position.y, 1000.0f);
}

} // namespace Sigma::Collision
