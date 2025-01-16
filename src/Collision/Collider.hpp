/**
 * @file Collider.hpp
 * @author dante
 * @date 1/16/2025
 *
 * @brief File to manage collisions between objects
 */
#pragma once

#include "../Object.hpp"

typedef unsigned int id_t; //TODO: remove when merge
struct Collision;
struct AEVec3;

struct RectCollider {
  FNFE::Transform * m_transform; //TODO: TEMP
  id_t m_id;
  std::vector<AEVec3> m_boxPoints;
  std::vector<AEVec3> m_boxScales;
  std::unordered_map<id_t, Collision> m_boxCollisionMap;
};

// TODO: change input to be the class of whereever the collider is stored
bool CollideObject(RectCollider &obj_a, RectCollider &obj_b);
void AddCollision(RectCollider &obj_a, RectCollider &obj_b);
void UpdateCollisionList(RectCollider &obj_a);

// DEBUG
void TestCollider();
void TestCollisionChecks();
void DrawRectCollider(const RectCollider &collider, unsigned color);