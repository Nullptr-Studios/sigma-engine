/**
 * @file DamageEvent.hpp
 * @author Alexey
 * @date 02/04/2025
 *
 * @brief Contains the definition of the Damage Event
 */

#pragma once
#include "Collision/CollisionEvent.hpp"

namespace Sigma::Damage {

enum DamageType : int {
  DAMAGE,
  THROW
};

/**
 * @class DamageEvent
 * @brief Event that is sent when an object deals damage to another object
 */
class DamageEvent : public Collision::CollisionEvent {
  float m_damageAmount;
  float m_knockbackAmount;
  DamageType m_damageType;

public:
  /**
   * @brief Creates and sets up a damage event
   *
   * @param receiver Object id that should receive the event
   * @param other Pointer to the object it has collided with
   * @param type Type of @c other 's collider
   * @param damageAmount Amount of damage dealt to receiver
   * @param knockback amount of knockback
   * @param damageType Type of damage dealt to receiver
   *
   */
  DamageEvent(id_t receiver, Object* other, Collision::ColliderType type, float damageAmount, const float knockback, DamageType damageType)
    : CollisionEvent(receiver, other, type), m_damageAmount(damageAmount),m_knockbackAmount(knockback), m_damageType(damageType) {}

  [[nodiscard]] float GetDamageAmount() const {return m_damageAmount;}  ///< @brief Gets the amount of damage dealt
  [[nodiscard]] float GetKnockbackAmount() const {return m_knockbackAmount;}  ///< @brief Gets the amount of knockback dealt
  [[nodiscard]] DamageType GetDamageType() const {return m_damageType;} ///< @brief Gets type of damage dealt
  void SetDamageType(const DamageType dType) {m_damageType = dType; } ///< @brief Sets damage type to either PHYSICAL or THROW

/**
   * @brief Overrides the @c ToString function to provide a detailed string representation of the event.
   * @return A string containing the event details.
   */
   [[nodiscard]] std::stringstream ToString() const override {
    std::stringstream ss;
    ss << "Damage Event between \"" << m_receiver << "\" and \"" << m_other->GetId() << "\"\n";
    return ss;
  }
  EVENT_CLASS_TYPE(DAMAGE)
};

}
