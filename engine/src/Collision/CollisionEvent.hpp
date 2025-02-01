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
#include "Objects/Object.hpp"

namespace sigma::Collision {

class CollisionEvent : public Event {
public:
  /**
   * @brief Creates and sets up a collision event
   *
   * @param receiver Object id that should receive the event
   * @param other Pointer to the object it has collided with
   * @param type Type of @c other 's collider
   */
  CollisionEvent(id_t receiver, Object* other, ColliderType type)
      : m_receiver(receiver), m_other(other), m_type(type) {}

  id_t GetReceiver() const { return m_receiver; } ///< @brief Gets the ID of the object that should receive the message
  Object* GetOther() const { return m_other; }    ///< @brief Gets the object that has collided with 
  ColliderType GetType() const { return m_type; } ///< @brief Gets the type of collision (collision, damage...)

  /**
   * @brief Overrides the @c ToString function to provide a detailed string representation of the event.
   * @return A string containing the event details.
   */
  [[nodiscard]] std::string ToString() const override {
    std::stringstream ss;
    ss << "Collision Event between \"" << m_receiver << "\" and \"" << m_other->GetId() << "\"\n";
    return ss.str();
  }

  EVENT_CLASS_TYPE(COLLISION)

private:
  id_t m_receiver;
  Object* m_other;
  ColliderType m_type;
};

}
