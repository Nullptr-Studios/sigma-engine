/**
 * @file Collider.hpp
 * @author dante
 * @date 1/16/2025
 *
 * @brief File to manage collisions between objects
 */

#pragma once
#include "Core.hpp"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace Sigma {
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
  private:
    float left = 0.5f; 
    float right = 0.5f; 
    float top = 0.5f; 
    float bottom = 0.5f;

    glm::vec2 offset = glm::vec2();
    
  public:
    /**
     * @brief Returns the sides of the box relative to a position
     * The values returned go as follow 
     *   - @c [0] left
     *   - @c [1] right
     *   - @c [2] top
     *   - @c [3] bottom
     */ 
    std::array<float, 4> GetSides(const glm::vec3& position) const {
      std::array<float, 4> vertices = {
        -left + position.x + offset.x,
         right + position.x + offset.x,
         top + position.y + offset.y,
        -bottom + position.y + offset.y
      };

      return vertices;
    }
    
    /**
     * @brief Sets the box colliders boundaries
     *
     * @param left_ Left side of the box
     * @param right_ Right side of the box
     * @param top_ Top part of the box
     * @param bottom_ Bottom part of the box
     * @param offset_ Offset to move the center of the box outside the center of the object
     */
    void Set(float left_, float right_, float top_, float bottom_, glm::vec2 offset_ = {0.0f, 0.0f}) {
      left = std::abs(left_);
      right = std::abs(right_);
      top = std::abs(top_);
      bottom = std::abs(bottom_);
      offset = offset_;
    }
  } box;
  float depth = 10.0f;
  
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
   * @param offset_ Offset to move the center of the box outside the center of the object
   */
  BoxCollider(ColliderFlag flag_, ColliderType type_, std::array<float, 4> size, glm::vec2 offset = {0.0f, 0.0f}) 
      : flag(flag_), type(type_) {
    box.Set(size[0], size[1], size[2], size[3], offset);
  }

  void DebugDraw(Actor* parent, color_t color) const;
};

} // namespace Collision
} // namespace Sigma
