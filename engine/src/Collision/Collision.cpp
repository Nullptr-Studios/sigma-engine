#include "Collision.hpp"
#include "CollisionEvent.hpp"
#include "DamageSystem/DamageEvent.hpp"
#include <immintrin.h>

namespace Sigma::Collision {

// Every day i think i prefer femboys a bit more than woman -x(probably) not d
// Thb the best is to date an enby, best of both worlds -x
void CollisionSystem::UpdateCollisions(ObjectMap* objects) {
  for (auto it1 = objects->begin(); it1 != objects->end(); ++it1) {
    auto& obj1 = it1->second;
    auto* col1 = obj1->GetCollider();
    if (!col1 || !col1->enabled) continue;

    const auto& pos1 = obj1->transform.position;
    const auto& box1 = col1->box;
    float depth1 = box1.GetDepth();
    auto sides1 = box1.GetSides(pos1);

    for (auto it2 = std::next(it1); it2 != objects->end(); ++it2) {
      auto& obj2 = it2->second;
      auto* col2 = obj2->GetCollider();
      if (!col2 || !col2->enabled) continue;

      if (!(col1->flag & col2->flag)) continue; // Early exit if no matching flag

      const auto& pos2 = obj2->transform.position;
      const auto& box2 = col2->box;
      float depth2 = box2.GetDepth();

      if (std::fabs(pos1.z - pos2.z) > (depth1 + depth2)) continue; // Z-depth check

      auto sides2 = box2.GetSides(pos2);

      // AABB collision check
      if (sides1[1] < sides2[0] || sides1[0] > sides2[1]) continue;
      if (sides1[2] < sides2[3] || sides1[3] > sides2[2]) continue;

      auto SendCollisionEvent = [&](auto* collider, id_t target_id) {
        if (collider->type == DAMAGE) {
          auto damageEvent = Damage::DamageEvent(target_id, collider->GetOwner(), collider->type, collider->damage, collider->damageType);
          SendEvent(damageEvent);
        } else {
          auto collisionEvent = CollisionEvent(target_id, collider->GetOwner(), collider->type);
          SendEvent(collisionEvent);
        }
      };

      // Dispatch events for both objects
      SendCollisionEvent(col1, obj2->GetId());
      SendCollisionEvent(col2, obj1->GetId());
    }
  }
}

} // namespace Sigma::Collision
