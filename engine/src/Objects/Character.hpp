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
  void Update(double delta) override;
  void Destroy() override { Actor::Destroy(); };

#pragma region MovementSystem
 
  void Move(glm::vec2 direction);
  void Jump();

#pragma endregion

  void BasicAttack();
  void SuperAttack();
 
  glm::mat3& GetTextureTransform() override;

  std::unique_ptr<Animation::AnimationComponent> m_animComp; ///< @brief Animation component

private:

  float m_movementYFloor = 0.0f; ///< @brief Y position of the floor

#pragma region MovementSystem
  void UpdateMovement(double delta);
 
  glm::vec2 velocity = glm::vec2(0.0f); ///< @brief character velocity

  float maxSpeed = 400.0f; ///< @brief character max velocity
  float accelerationRate = 25.0f; ///< @brief character acceleration
  float gravity = -5000.f;
  float friction = 2000.f; ///< @brief character friction
  float jumpVel = 2500.0f; ///< @brief character jump velocity
  float terminalVel = 1000.0f; ///< @brief character terminal velocity
  bool isJumping = false;  ///< @brief character jump status
 
  void PrintStatus() {};
#pragma endregion
 
#pragma region Combat

  void UpdateCombat(double delta);

  void ResetBasic() { m_basicCombo = 0; } ///< @brief Resets the basic attack combo to zero
  void ResetSuper() { m_superCombo = 0; } ///< @brief Resets the super attack combo to zero

  // TODO: I need animation callbacks for this pookie 😘
  bool m_isIdle = true;   ///< @brief Returns false if player is currently doing an animation (avoids spammability)
  bool m_inCombo = false; ///< @brief This stores whether the character can currently perform a combo or not

  unsigned char m_basicCombo = 0;  ///< @brief Combo status for default attack
  unsigned char m_superCombo = 0;  ///< @brief Combo status for super attack
  
  unsigned char m_basicMoveCount = 3;
  unsigned char m_superMoveCount = 4;
 
  double m_hitTimer = 0.0f;
  double m_restartTime = 1.6f;

#pragma endregion

};

}
