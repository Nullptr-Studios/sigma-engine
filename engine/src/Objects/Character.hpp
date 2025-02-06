/**
 * @file Character.hpp
 * @author a.goi
 * @date 1/16/2025
 *
 * @brief Characters are Actors that can be controlled
 */

#pragma once
#include "Actor.hpp"
#include "AnimationSystem/AnimationComponent.hpp"

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
  ~Character() override;

  void Init() override;
  void Start() override { Actor::Start(); };
  void Update(double delta) override {
    Actor::Update(delta);
    Character::UpdateMovement(delta);
    m_animComp->Update(delta);
  };
  void Destroy() override { Actor::Destroy(); };

#pragma region MovementSystem
  
  void Move(glm::vec2 direction);

  void Jump();

#pragma endregion
  
  
  glm::mat3* GetTextureTransform() override;

  std::unique_ptr<Animation::AnimationComponent> m_animComp; ///< @brief Animation component

  glm::vec2 velocity = glm::vec2(0.0f); ///< @brief character velocity

private:

  float m_movementYFloor = 0.0f; ///< @brief Y position of the floor

#pragma region MovementSystem
  void UpdateMovement(double delta);

  // TODO: Tweak variabñes -d
  float maxSpeed = 500.0f; ///< @brief character max velocity
  float accelerationRate = 25.f; ///< @brief character acceleration
  float gravity = -5000.f;
  float friction = 2000.f; ///< @brief character friction
  float jumpVel = 2500.0f; ///< @brief character jump velocity
  float terminalVel = 1000.0f; ///< @brief character terminal velocity
  bool isJumping = false; ///< @brief character jump status
  
  void PrintStatus() {};
#pragma endregion
  
  // TODO: Prob move this somewhere else -x
  unsigned char m_defCombo = 0;    ///< @brief Combo status for default attack
  unsigned char m_superCombo = 0;  ///< @brief Combo status for super attack
  unsigned char m_ultCombo = 0;    ///< @brief Combo status for ultimate attack

};

}
