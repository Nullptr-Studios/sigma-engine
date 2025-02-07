/**
 * @file Character.hpp
 * @author a.goi
 * @date 1/16/2025
 *
 * @brief Characters are Actors that can be controlled
 */

#pragma once
#include "Actor.hpp"
#include "DamageSystem/DamageEvent.hpp"

namespace Sigma {

/**
 * @class Character
 * @brief An Actor that can be controlled
 *
 * Characters inherit from @c Actors and can be controlled. They also introduce combo tracking and another
 * combat features.
 */
class Character : public Actor {
public:
  explicit Character(id_t id) : Actor(id) {}
  ~Character() override = default;

  void Init() override;
  void Start() override { Actor::Start(); };
  void Update(double delta) override { Actor::Update(delta); };
  void Destroy() override { Actor::Destroy(); };

  glm::mat3& GetTextureTransform() override;

  ANIMATION::AnimationComponent* m_animComp = nullptr;

  /**
   * @brief Event for modifying character health
   * @param e damage event reference
   */
  void OnDamage(Damage::DamageEvent &e);

  [[nodiscard]] float GetHealth() {return m_health;} ///< @brief returns amount of character health
  [[nodiscard]] bool GetAlive() {return m_isAlive;} ///< @brief returns whether character is alive or not
  void SetHealth(const float health) {m_health = health;} ///< @brief sets character health
  void SetAlive(const bool alive) {m_isAlive = alive;} ///< @brief sets character alive state


private:
  // Prob move this somewhere else -x
  float m_health = 100.0f; ///< @brief Amount of Health a character has, 100.0f by default
  bool m_isAlive = true; ///< @brief Determines whether the character is dead or alive, true by default
  unsigned char m_defCombo = 0;    ///< @brief Combo status for default attack
  unsigned char m_superCombo = 0;  ///< @brief Combo status for super attack
  unsigned char m_ultCombo = 0;    ///< @brief Combo status for ultimate attack

};

}
