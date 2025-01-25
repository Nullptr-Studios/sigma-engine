/**
 * @file Collider.hpp
 * @author dante
 * @date 1/16/2025
 *
 * @brief File to manage collisions between objects
 */
#pragma once
#include "Core.hpp"

/**
 * @enum ColliderFlag bitwise enum for detecting collisions
 */
enum ColliderFlag {
    PLAYER = BIT(0), ///<@brief binary 0001
    ENEMY = BIT(1), ///<@brief binary 0010
    UI = BIT(2), ///<@brief binary 0100
};

namespace FNFE {
class Actor;
}
struct AEVec3;
namespace FNFE::Collision {
struct Collision;

/**
 * @struct RectCollider a struct for the rect Collider
 */
struct RectCollider {
  unsigned flag; ///<@brief bitwise enum for what can the collider collide with
  std::vector<AEVec3> m_boxPoints; ///<@brief all the offsets for each box collider in the collider
  std::vector<AEVec3> m_boxScales; ///<@brief all the scales for each box collider in the collider
  std::unordered_map<id_t, Collision> m_boxCollisionDataMap; ///<@brief all the collision that are happening on this frame
};


/**
 * @brief check the collision between 2 objects and adds a collision
 * to their respective collisionMap
 * @param objA
 * @param objB
 * @return
 */
bool CollideObject(Actor *objA, FNFE::Actor *objB);

bool CollideObject(RectCollider *colliderA, RectCollider *colliderB);

/**
 * @brief adds a collision to both of the colliders and if there's already a collision between
 * the 2 objects change the collision to CollisionType::STAY
 *
 * @param objA
 * @param objB
 */
void AddCollision(Actor *objA, FNFE::Actor *objB);

/**
 * @brief Iterates around the collisionMap and updates all the collisions
 * to either on EXIT or delete it if already on exit
 * @param objA
 */
void UpdateCollisionList(RectCollider *objA);

// DEBUG
void TestCollider();
void TestCollisionChecks();
void DrawRectCollider(const Actor *obj, unsigned color);
} // namespace FNFE::Collision
