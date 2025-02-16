/**
 * @file CollisionEvent.hpp
 * @author Xein
 * @date 30/01/25
 *
 * @brief Contains the definition of the Collision Event
 */

#pragma once
#include "Collision/Collider.hpp"
#include "Events/Event.hpp"
#include "Objects/Object.hpp"

namespace Sigma::Collision {

/**
 * @class CollisionEvent
 * @brief Event used to handle all collision stuff
 *
 * To use this event see example:
 * @code
 * // The object will recieve the event on this function
 * bool OnCollision(CollisionEvent& e) {
 *   // Here you can use `e` to gather whatever you need of the event
 *   Object* receiver = e.GetReceiver; // This will return a pointer to the object it collied with
 *   ColliderType type = e.GetType; // This will return the type of the collider (collision, damage...)
 *   // Realistically, damage type events will not be called here but i cannot assure that since I'm not
 *   // in charge of the damage system
 *
 *   // The return value here indicates if the event should propagate or not
 *   return true;
 * }
 * @endcode
 */
class CollisionEvent : public Event {
public:
  /**
   * @brief Creates and sets up a collision event
   *
   * @param receiver Object id that should receive the event
   * @param other Pointer to the object it has collided with
   * @param type Type of @c other 's collider
   */
  CollisionEvent(id_t receiver, Object * other, ColliderType type)
      : m_receiver(receiver), m_other(other), m_type(type) {}

  id_t GetReceiver() const { return m_receiver; } ///< @brief Gets the ID of the object that should receive the message
  Object* GetOther() const { return m_other; }    ///< @brief Gets the object that has collided with 
  ColliderType GetType() const { return m_type; } ///< @brief Gets the type of collision (collision, damage...)

  /**
   * @brief Overrides the @c ToString function to provide a detailed string representation of the event.
   * @return A string containing the event details.
   */
  [[nodiscard]] std::stringstream ToString() const override {
    std::stringstream ss;
    ss << "Collision Event between \"" << m_receiver << "\" and \"" << m_other->GetId() << "\"\n";
    return ss;
  }

  EVENT_CLASS_TYPE(COLLISION)

protected:
  Object *m_other;
  id_t m_receiver;
  ColliderType m_type;
};

}