#include "Collision.hpp"
#include "CollisionEvent.hpp"

namespace Sigma::Collision {

// Every day i think i prefer femboys a bit more than woman -x(probably) not d
void CollisionSystem::UpdateCollisions(ObjectMap* objects) {
  for (auto it1 = objects->begin(); it1 != objects->end(); ++it1) {
    auto obj1 = it1->second;
    auto obj1_collider = obj1->GetCollider();
    if (!obj1_collider) continue; // Avoids if objects doesn't have a collider -x

    // Start the inner loop from the next element to avoid redundant checks -x
    for (auto it2 = std::next(it1); it2 != objects->end(); ++it2) {
      auto obj2 = it2->second;
      auto obj2_collider = obj2->GetCollider();
      if (!obj2_collider) continue;
      // Skips if the flags don't match -x
      if ((obj1_collider->flag & obj2_collider->flag) == 0) continue;
      
      // Depth check -x
      float z_distance = std::fabs(obj1->transform.position.z - obj2->transform.position.z);
      if (z_distance > obj1_collider->depth + obj2_collider->depth) continue;
      
      // index 0 is left, index 1 is right, index 2 is top, index 3 is bottom -x
      auto obj1_bounds = obj1_collider->box.GetSides(obj1->transform.position);
      auto obj2_bounds = obj2_collider->box.GetSides(obj2->transform.position);

      bool collisionX = (obj1_bounds[0] <= obj2_bounds[1]) && (obj1_bounds[1] >= obj2_bounds[0]);
      if (!collisionX) continue;

      bool collisionY = (obj1_bounds[3] <= obj2_bounds[2]) && (obj1_bounds[2] >= obj2_bounds[3]);
      if (!collisionY) continue;

      // Collision stuff -x
      CollisionEvent obj1_event = CollisionEvent(obj1->GetId(), obj2, obj2_collider->type);
      CollisionEvent obj2_event = CollisionEvent(obj2->GetId(), obj1, obj1_collider->type);

      SendEvent(obj1_event);
      SendEvent(obj2_event);
    }
  }
}

} // namespace Sigma
