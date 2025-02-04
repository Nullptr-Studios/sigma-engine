/**
 * @file Collision.hpp
 * @author dante
 * @date 1/16/2025
 *
 * @brief collision and collision data
 */

#pragma once
#include <utility>
#include "Objects/Object.hpp"

namespace Sigma::Collision {

class CollisionSystem {
  using EventCallbackFn = std::function<void(Event&)>; ///< Type alias for the event callback function
public:
  explicit CollisionSystem(EventCallbackFn callback) : m_callback(std::move(callback)) {}

  CollisionSystem(const CollisionSystem &) = delete;
  CollisionSystem &operator=(const CollisionSystem &) = delete;

  void UpdateCollisions(ObjectMap* objects);

private:
  void SendEvent(Event& e) { m_callback(e); }
  EventCallbackFn m_callback = nullptr;

};

} // namespace Sigma::Collision
