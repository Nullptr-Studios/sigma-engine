/**
 * @file Collider.cpp
 * @author dante
 * @date 1/16/2025
 *
 * @brief File to manage collisions between objects
 */

#include "Collider.hpp"
#include "Collision.hpp"
#include <Objects/Actor.hpp>

bool CollideObject(FNFE::Actor &obj_a, FNFE::Actor &obj_b) {
  RectCollider * collider_a = obj_a.GetCollider();
  RectCollider * collider_b = obj_a.GetCollider();
  if (!collider_a && !collider_b) {
    return false;
  }
  int rects_a = collider_a->m_boxPoints.size();
  int rects_b = collider_b->m_boxPoints.size();
  if (rects_a == 0 || rects_b == 0) {
    return false;
  }
  for (int a = 0; a < rects_a; a++) {
    for (int b = 0; b < rects_b; b++) {
      AEVec3 pos_a = collider_a->m_boxPoints[a] + obj_a.transform.position;
      AEVec3 scale_a = collider_a->m_boxScales[a];
      AEVec3 pos_b = collider_b->m_boxPoints[b] + obj_b.transform.position;
      AEVec3 scale_b = collider_b->m_boxScales[b];
      DrawRectangleAt(pos_a, scale_a, AE_COLORS_GREEN);
      DrawRectangleAt(pos_b, scale_b, AE_COLORS_GREEN);
      if (RectOnRect(pos_a, scale_a, pos_b, scale_b)) {
        AddCollision(obj_a, obj_b);
        return true;
      }
    }
  }
  return false;
}

void AddCollision(FNFE::Actor &obj_a, FNFE::Actor &obj_b) {
  RectCollider * collider_a = obj_a.GetCollider();
  RectCollider * collider_b = obj_a.GetCollider();
  if (collider_a->m_boxCollisionMap.contains(obj_b.GetId())) {
    collider_a->m_boxCollisionMap.at(obj_b.GetId()).m_type = CollisionType::STAY;
    collider_b->m_boxCollisionMap.at(obj_a.GetId()).m_type = CollisionType::STAY;
  } else {
    collider_a->m_boxCollisionMap.insert({obj_b.GetId(), {CollisionType::ENTER, obj_b.GetId()}});
    collider_b->m_boxCollisionMap.insert({obj_a.GetId(), {CollisionType::ENTER, obj_a.GetId()}});
  }
}

void UpdateCollisionList(RectCollider &obj) {
    for (auto it = obj.m_boxCollisionMap.begin(); it != obj.m_boxCollisionMap.end();) {
        if (it->second.m_type == CollisionType::STAY) {
            it->second.m_type = CollisionType::EXIT;
            ++it;
        } else if (it->second.m_type == CollisionType::ENTER) {
            it->second.m_type = CollisionType::EXIT;
            ++it;
        } else if (it->second.m_type == CollisionType::EXIT) {
            it = obj.m_boxCollisionMap.erase(it); // Erase and update iterator
        } else {
            ++it;
        }
    }
}


#pragma region TEST
void DrawRectCollider(const FNFE::Actor obj, unsigned color) {
  RectCollider * collider = obj.GetCollider();
  if (!collider) {
    return;
  }
  int rectangles = collider->m_boxPoints.size();
  for (int i = 0; i < rectangles; i++) {
    AEVec2 pos = {obj.transform.position.x, obj.transform.position.y};
    pos += {collider->m_boxPoints[i].x, collider->m_boxPoints[i].y};
    AEVec2 scale = {collider->m_boxScales[i].x, collider->m_boxScales[i].y};
    DrawRectangleAt(pos, scale, color);
  }
}
#pragma endregion