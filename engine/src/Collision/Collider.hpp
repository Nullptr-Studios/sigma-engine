/**
 * @file Collider.hpp
 * @author dante
 * @date 1/16/2025
 *
 * @brief File to manage collisions between objects
 */

#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace FNFE {
class Actor;

namespace Collision {

/**
 * @enum ColliderFlag 
 * @brief Bitwise enum for detecting collisions
 */
enum ColliderFlag {
  PLAYER = BIT(0),  ///< @brief binary 0001
  ENEMY  = BIT(1),  ///< @brief binary 0010
  UI     = BIT(2),  ///< @brief binary 0100
  BULLET = BIT(3),  ///< @brief binary 1000
};

enum ColliderType {
  COLLISION,
  DAMAGE
};

/**
 * @struct BoxCollider 
 * @brief A struct for a rectangular sized collider
 */
struct BoxCollider {
  bool enabled = true; ///< @brief Checks if the collision should be calculated or not

  ColliderFlag flag; ///< @brief Bitwise enum for what can the collider collide with
  ColliderType type; ///< @brief Enum that stores if the collider is used for collision or for dealing damage
  
  /**
   * @struct Box
   * @brief Array that stores the box size
   * Since the triangles cannot rotate, I simply store the displacement of the Left, Right, Top and Bottom
   * Positions of the sides are absolute
   *
   * @c GetPoints can be used to get the actual coordinates of the 4 points of the square
   */ 
  struct Box {
    float left = 0.5f; 
    float right = 0.5f; 
    float top = 0.5f; 
    float bottom = 0.5f;

    /**
     * @brief Returns the coordinates of the rectangle
     * The coordinates are returned in this order: 
     *   - @c [0] Top Left
     *   - @c [1] Top Right
     *   - @c [2] Bottom Left
     *   - @c [3] Bottom Right
     */ 
    std::array<glm::vec2, 4> GetPoints(const glm::vec3& position) const {
      std::array<glm::vec2, 4> vertices = {
        glm::vec2(position.x - fabs(left), position.y + fabs(top)),
        glm::vec2(position.x + fabs(right), position.y + fabs(top)),
        glm::vec2(position.x - left, position.y - bottom),
        glm::vec2(position.x + right, position.y - bottom)
      };

      return vertices;
    }
  } box;
  float depth = 0.1f;
  
  /**
   * @brief Initializes collider by setting the flags and the type
   * The collider is generated as a square of 100x100 pixels by default
   *
   * @param flag_ Sets with which colliders it should interact
   * @param type_ Sets the collider type (collision or damage)
   */ 
  BoxCollider(ColliderFlag flag_, ColliderType type_) : flag(flag_), type(type_) {}
  
  /**
   * @brief initializes the collider by setting the flags and the type and specifies its size
   * the order of the specification is left, right, top bottom
   *
   * @param flag_ sets with which colliders it should interact
   * @param type_ sets the collider type (collision or damage)
   * @param size  sets the 4 floats that define the size of the collider (l, r, t, b)
   */
  BoxCollider(ColliderFlag flag_, ColliderType type_, std::array<float, 4> size) 
      : flag(flag_), type(type_) {
    box.left   = size[0];
    box.right  = size[1];
    box.top    = size[2];
    box.bottom = size[3];
  }

  void DebugDraw(Actor* parent, color_t color) const;
};

} // namespace Collision
} // namespace FNFE
