/**
 * @file CollisionEvent.hpp
 * @author Xein
 * @date 30/01/25
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include "Events/Event.hpp"
#include "Collider.hpp"

namespace FNFE {
class Object;

namespace Collision {

class CollisionEvent : public Event {
public:
  CollisionEvent(Object* collider, ColliderFlag target) : m_collider(collider), m_target(target) {}

  EVENT_CLASS_TYPE(COLLISION)

private:
  Object* m_collider;
  ColliderFlag m_target;
  
};

}
}
