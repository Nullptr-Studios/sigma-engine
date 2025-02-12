//
// Created by dariormt on 08/02/2025.
//

#pragma once

#include "Actor.hpp"
#include "Collision/OneHitCollider.hpp"
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
  // Every day i think i prefer femboys a bit more -Copilot 12/02/2025 00:58:43
  // TODO: add cooldown to damage -d
  virtual void OnDamage(const Damage::DamageEvent &e)
  {

    // Avoid self collisions
    if (e.GetOther() == this) {
      std::cout << GetName() << " tried to damage itself\n";
      return;
    }
      
    
    float currentHealth = m_health - e.GetDamageAmount();
    SetHealth(std::max(0.0f, currentHealth));
    if (currentHealth <= 0) {
      m_isAlive = false;
      OnDed();
    }
    
  };

  virtual void OnDed() {
    std::cout << GetName() << " is ded\n";
  };
  virtual void OnHeal(){};

  void Init() override {
    Actor::Init();
    
    // creates default collider
    m_collider = std::make_unique<Collision::BoxCollider>(Collision::PLAYER | Collision::ENEMY, Collision::COLLISION);
  };
  
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
  void SetHealth(const float health) {
    if (health > m_health)
      OnHeal();
    m_health = health;
  }
  
  void SetAlive(const bool alive) {m_isAlive = alive;} ///< @brief sets character alive state

protected:
  float m_health = 100.0f;
  bool m_isAlive = true;

  
};

} // Sigma

