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

/**
 * @brief a struct for the rect Collider
 */
struct RectCollider {
  FNFE::Transform * m_transform; //TODO: TEMP
  id_t m_id;
  /**
   * @brief all the offsets for each box collider in the collider
   */
  std::vector<AEVec3> m_boxPoints;
  /**
   * @brief all the scales for each box collider in the collider
   */
  std::vector<AEVec3> m_boxScales;
  /**
   *
   */
  std::unordered_map<id_t, Collision> m_boxCollisionMap;
};

// TODO: change input to be the class of whereever the collider is stored

/**
 * @brief check the collision between 2 objects and adds a collision
 * to their respective collisionMap
 * @param obj_a
 * @param obj_b
 * @return
 */
bool CollideObject(RectCollider &obj_a, RectCollider &obj_b);

/**
 * @brief adds a collision to both of the colliders and if there's already a collision between
 * the 2 objects change the collision to CollisionType::STAY
 *
 * @param obj_a
 * @param obj_b
 */
void AddCollision(RectCollider &obj_a, RectCollider &obj_b);

/**
 * @brief Iterates around the collisionMap and updates all the collisions
 * to either on EXIT or delete it if already on exit
 * @param obj_a
 */
void UpdateCollisionList(RectCollider &obj_a);

// DEBUG
void TestCollider();
void TestCollisionChecks();
void DrawRectCollider(const RectCollider &collider, unsigned color);