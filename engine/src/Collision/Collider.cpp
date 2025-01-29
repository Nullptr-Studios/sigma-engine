#include "Collider.hpp"
#include <Objects/Actor.hpp>
#include "Collision.hpp"

namespace FNFE {

bool Collision::CollideObject(FNFE::Actor *objA, FNFE::Actor *objB) {
  RectCollider *colliderA = objA->GetCollider();
  RectCollider *colliderB = objB->GetCollider();
  if (!colliderA || !colliderB) {
    return false;
  }
  if (!(colliderA->flag & colliderB->flag)) {
    return false;
  }
  int rectsA = colliderA->m_boxPoints.size();
  int rectsB = colliderB->m_boxPoints.size();
  if (rectsA == 0 || rectsB == 0) {
    return false;
  }
  for (int a = 0; a < rectsA; a++) {
    for (int b = 0; b < rectsB; b++) {
      glm::vec3 posA = colliderA->m_boxPoints[a] + objA->transform.position;
      glm::vec3 scaleA = colliderA->m_boxScales[a];
      glm::vec3 posB = colliderB->m_boxPoints[b] + objB->transform.position;
      glm::vec3 scaleB = colliderB->m_boxScales[b];
      if (RectOnRect(posA, scaleA, posB, scaleB)) {
        AddCollision(objA, objB);
        return true;
      }
    }
  }
  return false;
}

void Collision::AddCollision(FNFE::Actor *objA, FNFE::Actor *objB) {
  RectCollider *colliderA = objA->GetCollider();
  RectCollider *colliderB = objB->GetCollider();
  if (colliderA->m_boxCollisionDataMap.contains(objB->GetId())) {
    colliderA->m_boxCollisionDataMap.at(objB->GetId()).m_type = CollisionType::STAY;
    colliderB->m_boxCollisionDataMap.at(objA->GetId()).m_type = CollisionType::STAY;
  } else {
    colliderA->m_boxCollisionDataMap.insert({objB->GetId(), {CollisionType::ENTER, objB->GetId()}});
    colliderB->m_boxCollisionDataMap.insert({objA->GetId(), {CollisionType::ENTER, objA->GetId()}});
  }
}

void Collision::UpdateCollisionList(RectCollider *obj) {
  if (obj == nullptr)
    return;

  for (auto it = obj->m_boxCollisionDataMap.begin(); it != obj->m_boxCollisionDataMap.end();) {
    if (it->second.m_type != CollisionType::STAY) {
      it->second.m_type = CollisionType::EXIT;
      ++it;
    } else if (it->second.m_type == CollisionType::ENTER) {
      it->second.m_type = CollisionType::EXIT;
      ++it;
    } else if (it->second.m_type == CollisionType::EXIT) {
      // Erase and update iterator
      it = obj->m_boxCollisionDataMap.erase(it);
    } else {
      ++it;
    }
  }
}

#pragma region TEST

void Collision::DrawRectCollider(const FNFE::Actor *obj, unsigned color) {
  RectCollider *collider = obj->GetCollider();
  if (!collider)
    return; // Sanity check -x

  int rectangles = collider->m_boxPoints.size();
  for (int i = 0; i < rectangles; i++) {
    glm::vec2 pos = {obj->transform.position.x / 2, -obj->transform.position.y / 2};
    pos += glm::vec2(collider->m_boxScales[i].x / 2, -collider->m_boxScales[i].y / 2);
    glm::vec2 scale = {collider->m_boxScales[i].x, collider->m_boxScales[i].y};
    DrawRectangleAt(pos, scale, color);
  }
}

#pragma endregion

} // namespace FNFE
