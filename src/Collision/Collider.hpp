/**
 * @file Collider.hpp
 * @author dante
 * @date 1/16/2025
 *
 * @brief File to manage collisions between objects
 */
#pragma once
#include "Core.hpp"


namespace FNFE {
  class Actor;
}
struct Collision;
struct AEVec3;

/**
 * @struct RectCollider a struct for the rect Collider
 */
struct RectCollider {
  /**
   * @brief all the offsets for each box collider in the collider
   */
  std::vector<AEVec3> m_boxPoints;
  /**
   * @brief all the scales for each box collider in the collider
   */
  std::vector<AEVec3> m_boxScales;
  /**
   * @brief all the collision that are happening on this frame
   */
  std::unordered_map<FNFE::id_t, Collision> m_boxCollisionMap;
};


/**
 * @brief check the collision between 2 objects and adds a collision
 * to their respective collisionMap
 * @param obj_a
 * @param obj_b
 * @return
 */
bool CollideObject(FNFE::Actor &obj_a, FNFE::Actor &obj_b);

/**
 * @brief adds a collision to both of the colliders and if there's already a collision between
 * the 2 objects change the collision to CollisionType::STAY
 *
 * @param obj_a
 * @param obj_b
 */
void AddCollision(FNFE::Actor &obj_a, FNFE::Actor &obj_b);

/**
 * @brief Iterates around the collisionMap and updates all the collisions
 * to either on EXIT or delete it if already on exit
 * @param obj_a
 */
void UpdateCollisionList(RectCollider &obj_a);

// DEBUG
void TestCollider();
void TestCollisionChecks();
void DrawRectCollider(const FNFE::Actor obj, unsigned color);
