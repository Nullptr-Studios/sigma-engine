/**
 * @file Collision.hpp
 * @author dante
 * @date 1/16/2025
 *
 * @brief collision and collision data
 */

#pragma once
#include "Objects/Object.hpp"

namespace sigma::Collision {

class CollisionSystem {
public:
  CollisionSystem() = default;

  CollisionSystem(const CollisionSystem &) = delete;
  CollisionSystem &operator=(const CollisionSystem &) = delete;

  void UpdateCollisions(ObjectMap* objects);
};

} // namespace sigma::Collision
