#include "Collision.hpp"

namespace sigma::Collision {

// Every day i think i prefer femboys a bit more than woman -d(probably)
void CollisionSystem::UpdateCollisions(ObjectMap* objects) {
  for (auto it1 = objects->begin(); it1 != objects->end(); ++it1) {
    auto obj1 = it1->second;
    auto obj1collider = obj1->GetCollider();
    if (!obj1collider) continue; // Avoids if objects doesn't have a collider -x

    // Start the inner loop from the next element to avoid redundant checks -x
    for (auto it2 = std::next(it1); it2 != objects->end(); ++it2) {
      auto obj2 = it2->second;
      auto obj2collider = obj2->GetCollider();
      if (!obj2collider) continue;
      
      // Depth check -x
      float z_distance = std::fabs(obj1->transform.position.z - obj2->transform.position.z);
      if (z_distance > obj1collider->depth + obj2collider->depth) continue;
      
      // index 0 is left, index 1 is right, index 2 is top, index 3 is bottom -x
      // TODO: This could be optimized so that if x fails it doesn't calculate y -x
      auto obj1bounds = obj1collider->box.GetSides(obj1->transform.position);
      auto obj2bounds = obj2collider->box.GetSides(obj2->transform.position);

      bool collisionX = (obj1bounds[0] <= obj2bounds[1]) && (obj1bounds[1] >= obj2bounds[0]);
      bool collisionY = (obj1bounds[3] <= obj2bounds[2]) && (obj1bounds[2] >= obj2bounds[3]);

      if (collisionX && collisionY) {
        // Collision stuff -x
      }
    }
  }
}

} // namespace sigma
