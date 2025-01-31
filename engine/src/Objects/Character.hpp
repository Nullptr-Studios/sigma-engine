/**
 * @file Character.hpp
 * @author a.goi
 * @date 1/16/2025
 *
 * @brief Characters are Actors that can be controlled
 */

#pragma once
#include "Actor.hpp"

namespace FNFE {

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

private:
  // Prob move this somewhere else -x
  unsigned char m_defCombo = 0;    ///< @brief Combo status for default attack
  unsigned char m_superCombo = 0;  ///< @brief Combo status for super attack
  unsigned char m_ultCombo = 0;    ///< @brief Combo status for ultimate attack

};

}
