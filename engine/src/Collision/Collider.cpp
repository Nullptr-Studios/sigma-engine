#include "Collider.hpp"
#include <Objects/Actor.hpp>
#include "aecore/AEGraphics.h"

namespace Sigma::Collision {

// I think this should work but it's untested -x
// Thanks alpha for having AEGfxRectMinMax -x
void BoxCollider::DebugDraw(Actor* parent, unsigned color = AE_COLORS_GREEN) const {
  auto points = box.GetSides(parent->transform.position);
  AEGfxRectMinMax(points[0], points[2], points[1], points[3], 0.0f, color); 
}

} // namespace Sigma
