/**
 * @file Collider.cpp
 * @author dante
 * @date 1/16/2025
 *
 * @brief File to manage collisions between objects
 */

#include "Collider.hpp"

#include <iostream>

#include "Collision.hpp"

bool CollideObject(RectCollider &obj_a, RectCollider &obj_b) {
  int rects_a = obj_a.m_boxPoints.size();
  int rects_b = obj_b.m_boxPoints.size();
  if (rects_a == 0 || rects_b == 0) {
    return false;
  }
  for (int a = 0; a < rects_a; a++) {
    for (int b = 0; b < rects_b; b++) {
      // TODO: Change to be transform of class instead collider used to offset
      AEVec3 pos_a = obj_a.m_boxPoints[a] + obj_a.m_transform->position;
      AEVec3 scale_a = obj_a.m_boxScales[a];
      AEVec3 pos_b = obj_b.m_boxPoints[b] + obj_b.m_transform->position;
      AEVec3 scale_b = obj_b.m_boxScales[b];
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

void AddCollision(RectCollider &obj_a, RectCollider &obj_b) {
  if (obj_a.m_boxCollisionMap.contains(obj_b.m_id)) {
    obj_a.m_boxCollisionMap.at(obj_b.m_id).m_type = CollisionType::STAY;
    obj_b.m_boxCollisionMap.at(obj_a.m_id).m_type = CollisionType::STAY;
  } else {
    obj_a.m_boxCollisionMap.insert({obj_b.m_id, {CollisionType::ENTER, obj_b.m_id}});
    obj_b.m_boxCollisionMap.insert({obj_a.m_id, {CollisionType::ENTER, obj_a.m_id}});
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
void Print(RectCollider &obj) {
  for (auto it = obj.m_boxCollisionMap.begin(); it != obj.m_boxCollisionMap.end(); ++it) {
    std::cout << it->first << "  ";
    Print(it->second.m_type);
  }
}
void TestCollisionChecks() {
  RectCollider obj_a{};
  RectCollider obj_b{};
  AddCollision(obj_a, obj_b);
  Print(obj_a);
  AddCollision(obj_a, obj_b);
  Print(obj_a);
  UpdateCollisionList(obj_a);
  Print(obj_a);
  UpdateCollisionList(obj_a);
  Print(obj_a);
}
void TestCollider() {

  FNFE::Transform transform_a{};
  FNFE::Transform transform_b{};
  MousePositionData data = AEGetMouseData();
  transform_a.position = {data.position.x, data.position.y, 0};
  RectCollider obj_a{};
  obj_a.m_transform = &transform_a;
  obj_a.m_id = 1;
  obj_a.m_boxPoints.push_back({0, 0, 0});
  obj_a.m_boxScales.push_back({20, 20, 20});
  obj_a.m_boxPoints.push_back({20, 20, 0});
  obj_a.m_boxScales.push_back({20, 20, 20});
  RectCollider obj_b{};
  obj_b.m_transform = &transform_b;
  obj_b.m_id = 2;
  obj_b.m_boxPoints.push_back({0, 0, 0});
  obj_b.m_boxScales.push_back({20, 20, 20});
  obj_b.m_boxPoints.push_back({20, 20, 0});
  obj_b.m_boxScales.push_back({20, 20, 20});

  unsigned color = AE_COLORS_BLUE;
  if (CollideObject(obj_a, obj_b)) {
    color = AE_COLORS_RED;
  }

  DrawRectCollider(obj_a, color);
  DrawRectCollider(obj_b, color);
}


void DrawRectCollider(const RectCollider &collider, unsigned color) {
  int rectangles = collider.m_boxPoints.size();
  for (int i = 0; i < rectangles; i++) {
    AEVec2 pos = {collider.m_transform->position.x, collider.m_transform->position.y};
    pos += {collider.m_boxPoints[i].x, collider.m_boxPoints[i].y};
    AEVec2 scale = {collider.m_boxScales[i].x, collider.m_boxScales[i].y};
    DrawRectangleAt(pos, scale, color);
  }
}
#pragma endregion