/**
 * @file Collision.hpp
 * @author dante
 * @date 1/16/2025
 *
 * @brief collision and collision data
 */
#pragma once
#include "Core.hpp"


/**
 * @brief calculates if 2 rectangles in the 3rd dimension are colliding
 * position relative to center of the rectangle
 * @param pos_a Position of Rectangle A
 * @param scale_a Width and Height and Depth of Rectangle A
 * @param pos_b Position of Rectangle B
 * @param scale_b Width and Height and Depth of Rectangle B
 * @return if the rectangles collide
 */
bool RectOnRect(AEVec3 &pos_a, AEVec3 &scale_a, AEVec3 &pos_b, AEVec3 &scale_b);

/**
 * @enum CollisionType Collision Types
 */
enum CollisionType {

  /**
   * @brief The object has collided with this object
   */
  ENTER,
  /**
   * @brief the object has been colliding with this object
   */
  STAY,
  /**
   * @brief the object has just exited colliding with the object
   */
  EXIT
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
