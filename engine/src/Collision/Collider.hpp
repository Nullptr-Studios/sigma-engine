/**
 * @file Collider.hpp
 * @author dante and Xein
 * @date 1/16/2025
 *
 * @brief File to manage collisions between objects
 */

// This is spaghetti code, also I wanna end my life -d

#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "Core.hpp"

#include "glm/vec4.hpp"

// The code was crosslinked, using forward declarations -d
// #include "DamageSystem/DamageEvent.hpp"

namespace Sigma {
class Object;
}
namespace Sigma {
class Actor;

namespace Damage {
enum DamageType : int;
}

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
  
  Damage::DamageType damageType; ///< @brief Type of damage the collider does if it's a damage collider

  /**
   * @brief Initializes collider by setting the flags and the type
   * The collider is generated as a square of 100x100 pixels by default
   *
   * @param flag_ Sets with which colliders it should interact
   * @param type_ Sets the collider type (collision or damage)
   */ 
  BoxCollider(int flag_, ColliderType type_) : type(type_) {
    flag = static_cast<ColliderFlag>(flag_);
    
  }
  
  /**
   * @brief initializes the collider by setting the flags and the type and specifies its size
   * the order of the specification is left, right, top bottom
   *
   * @param flag_ sets with which colliders it should interact
   * @param type_ sets the collider type (collision or damage)
   * @param size  sets the 4 floats that define the size of the collider (l, r, t, b)
   * @param offset_ Offset to move the center of the box outside the center of the object
   */
  BoxCollider(int flag_, ColliderType type_, std::array<float, 4> size, float depth, glm::vec2 offset = {0.0f, 0.0f}) 
      : type(type_) {
    flag = static_cast<ColliderFlag>(flag_);
    box.Set(size[0], size[1], size[2], size[3], depth, offset);
  }

  /**
   * @struct Box
   * @brief stores the box size
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
    
    float depth = 25.0f; ///< @brief Z Depth of the collider for 2.5D
    

    glm::vec2 offset = glm::vec2();
    
  public:

    
    /**
     * @brief Sets the box colliders boundaries
     *
     * @param left Left side of the box
     * @param right Right side of the box
     * @param top Top part of the box
     * @param bottom Bottom part of the box
     * @param offset Offset to move the center of the box outside the center of the object
     */
    void Set(float left, float right, float top, float bottom, float depth, glm::vec2 offset = {0.0f, 0.0f}) {
      this->left = std::abs(left);
      this->right = std::abs(right);
      this->top = std::abs(top);
      this->bottom = std::abs(bottom);
      this->depth = depth;
      this->offset = offset;
    }
    

    void Set(glm::vec3 scale, glm::vec2 offset = {0, 0}) {
      this->left = scale.x/2;
      this->right = scale.x/2;
      this->top = scale.y/2;
      this->bottom = scale.y/2;
      this->depth = scale.z;
      this->offset = offset;
    }

    
    float GetDepth() const { return depth; }
    
    /**
     * @brief Returns the sides of the box relative to a position
     * The values returned go as follow 
     *   - @c [0] left
     *   - @c [1] right
     *   - @c [2] top
     *   - @c [3] bottom
     */ 
    [[nodiscard]] std::vector<float> GetSides(const glm::vec3& position) const {
      std::vector<float> vertices = {
        -left + position.x + offset.x,
         right + position.x + offset.x,
         top + position.y + offset.y,
        -bottom + position.y + offset.y
      };

      return vertices;
    }

    [[nodiscard]] glm::vec2 GetScale() const { return glm::vec2(left+right, top+bottom); }

    [[nodiscard]] glm::vec2 GetOffset() const { return offset; }
    
  } box;

  //i really like the collider damage here -x
  //I fucking hate this -d
  // me too buddy i hate this so much -d

  float damage = 0.0f; ///< @brief Damage the attack does (only useful for DMG type colliders)
  float knockback = 0.0f;

  void SetColliderFlags(int flag) { this->flag = static_cast<ColliderFlag>(flag); }
  void SetColliderType(ColliderType type) { this->type = type; }
  void SetDamageType(Damage::DamageType dType) { damageType = dType; }

  // Ownership of the collider
  void SetOwner(Object* owner) { this->m_owner = owner; }
  [[nodiscard]] Object* GetOwner() const { return m_owner; }


  /**
   * @brief Creates an object to debug colliders
   * There are some images that can help you with debugging, by default the green color is used but you can also use 
   * @code debug_red.png, debug_yellow.png, debug_blue.png, debug_pink.png @endcode. 
   *
   * @param debug Actor to use for debugging
   * @param parent Parent of the collider
   * @param path Path for the debug texture (debug_green.png by default)
   */
  void DebugDraw(Actor* debug, Actor* parent, const char* path = "assets/core/debug_green.png") const;

private:
  Object* m_owner; ///< @brief Owner of the collider
};

}
}
