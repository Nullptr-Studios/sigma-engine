//
// Created by dariormt on 08/02/2025.
//

#pragma once

#include "Actor.hpp"
#include "DamageSystem/DamageEvent.hpp"

namespace Sigma {

/**
 * @class Damageable
 * @brief An Actor that can take damage
 */
class Damageable : public Sigma::Actor {
  
public:
  explicit Damageable(id_t id) : Actor(id) {}
  ~Damageable() override = default;

  /**
   * @brief Event for modifying character health
   * @param e damage event reference
   */
  virtual void OnDamage(const Damage::DamageEvent &e)
  {
    float currentHealth = m_health - e.GetDamageAmount();
    SetHealth(std::max(0.0f, currentHealth));
    if (currentHealth <= 0) {
      m_isAlive = false;
      OnDed();
    }
    
  };

 /**
   * @brief Sets the health of the character
   *
   * @param health Health to set
   */

  virtual void OnDed() {};
  void OnHeal(float health) { SetHealth(m_health + health); };

  void Init() override { Actor::Init(); };
  void Start() override {Actor::Start();};
  void Update(double delta) override {Actor::Update(delta);};
  void Draw() override {Actor::Draw();};
  void Destroy() override {Actor::Destroy();};

  [[nodiscard]] float GetHealth() const { return m_health;} ///< @brief returns amount of character health
  [[nodiscard]] bool GetAlive() const { return m_isAlive;} ///< @brief returns whether character is alive or not

  /**
   * @brief Sets the health of the character
   *
   * @param health Health to set
   */

  virtual void SetHealth(float health) {
    health = std::min(health, m_maxHealth);
    m_health = health;
  }
  
  void SetAlive(const bool alive) {m_isAlive = alive;} ///< @brief sets character alive state

protected:
  float m_maxHealth = 100.0f;
  float m_health = m_maxHealth;
  bool m_isAlive = true;

};

} // Sigma