/**
 * @file Collision.hpp
 * @author dante
 * @date 1/16/2025
 *
 * @brief collision and collision data
 */
#pragma once
#include "Core.hpp"

namespace FNFE::Collision {

/**
 * @brief calculates if 2 rectangles in the 3rd dimension are colliding
 * position relative to center of the rectangle
 * @param posA Position of Rectangle A
 * @param scaleA Width and Height and Depth of Rectangle A
 * @param posB Position of Rectangle B
 * @param scaleB Width and Height and Depth of Rectangle B
 * @return if the rectangles collide
 */
bool RectOnRect(AEVec3 &posA, AEVec3 &scaleA, AEVec3 &posB, AEVec3 &scaleB);

/**
 * @enum CollisionType Collision Types
 */
enum CollisionType {
  ENTER, ///< @brief The object has collider with this object
  STAY, ///< @brief the object has been colliding with this object
  EXIT ///< @brief the object has just exited colliding with the object
};

/**
 * @struct Collision holds all the information of a collision
 */
struct Collision {
  /**
   * @brief the type of collision
   */
  CollisionType m_type;
  /**
   * @brief the id of the object of collision
   */
  FNFE::id_t m_object;
};

// DEBUG
void TestRect();
void Print(CollisionType type);
void DrawRectangleAt(AEVec2 pos, AEVec2 scale, unsigned color);
void DrawRectangleAt(AEVec3 pos, AEVec3 scale, unsigned color);
} // namespace FNFE::Collision
