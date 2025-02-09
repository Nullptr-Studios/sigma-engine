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
#include "DamageSystem/DamageEvent.hpp"
#include "json.hpp"

namespace Sigma {
using json = nlohmann::json;

namespace Combat{

/**
 * @enum MoveType
 * @brief Holds all the diferent types of moves
 */ 
enum MoveType {
  DMG = 0, ///< @brief This move only does damage (default)
  GRB = 1, ///< @brief This move grabs the enemy as well
  THR = 2  ///< @brief This move throws the enemy and stuns them
};

/**
 * @brief Converts string to @c MoveType
 * If not valid it returns type DMG as a default
 * @param value String to convert
 * @return MoveType value
 */
inline MoveType GetMoveType(const std::string& value) {
  if (value == "grb" || value == "GRB") return GRB;
  else if (value == "thr" || value == "THR") return THR;
  return DMG;
}

/**
 * @class Move
 * @brief Data required for a single move of every combo
 */
struct Move {
  MoveType type;
  float damage;
  glm::vec2 colliderOffset{};
  glm::vec3 colliderSize{};
  std::string animationName;

  /**
   * @brief Empty constructor
   * This generates an empty move struct, only used for develompemt purposes
   */
  Move() {
    type = DMG; damage = 10.0f;
    colliderOffset = glm::vec2(0.0f);
    colliderSize = glm::vec3(0.0f);
    animationName = "";
  }

  /**
   * @brief Creates a move and initializes it
   * A move contains all the required information for a single attack in a combo
   * This struct also holds all the collider and animation parameter it needs
   *
   * @param type Defines the behaviour of the move
   * @param damage Defines the damage the move default
   * @param offset Defines the offset of the collider
   * @param size Defines the collider
   * @param animation Path of the animation for the move
   */
  Move(const MoveType type, const float damage, const glm::vec2 offset, const glm::vec3 size, const std::string& animation) {
    this->type = type;
    this->damage = damage;
    colliderOffset = offset;
    colliderSize = size;
    animationName = animation;
  }
};

}

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
  void Start() override;
  void Update(double delta) override;
  void Destroy() override { Actor::Destroy(); };

  virtual void Serialize();
  void SetJsonPath(const std::string& path) { m_jsonPath = path; }

#pragma region MovementSystem
  void Move(glm::vec2 direction);
  void Jump();
#pragma endregion

  void BasicAttack();
  void SuperAttack();
 
  glm::mat3& GetTextureTransform() override;

  std::unique_ptr<Animation::AnimationComponent> m_animComp; ///< @brief Animation component

protected:
  json j = nullptr;

/**
   * @brief Event for modifying character health
   * @param e damage event reference
   */
  virtual void OnDamage(Damage::DamageEvent &e);

  [[nodiscard]] float GetHealth() {return m_health;} ///< @brief returns amount of character health
  [[nodiscard]] bool GetAlive() {return m_isAlive;} ///< @brief returns whether character is alive or not
  void SetHealth(const float health) {m_health = health;} ///< @brief sets character health
  void SetAlive(const bool alive) {m_isAlive = alive;} ///< @brief sets character alive state

private:
  float m_health = 100.0f;
  bool m_isAlive = true;
  float m_movementYFloor = 0.0f; ///< @brief Y position of the floor
  std::string m_jsonPath;

#pragma region MovementSystem
  void UpdateMovement(double delta);
 
  glm::vec2 velocity = glm::vec2(0.0f); ///< @brief character velocity

  float maxSpeed = 400.0f; ///< @brief character max velocity
  float accelerationRate = 25.0f; ///< @brief character acceleration
  float gravity = -5000.f;
  float friction = 2000.f; ///< @brief character friction
  float jumpVel = 2500.0f; ///< @brief character jump velocity
  float terminalVel = 1000.0f; ///< @brief character terminal velocity
  bool  isJumping = false; ///< @brief character jump status
 
  void PrintStatus() {};
#pragma endregion
 
#pragma region Combat
  void UpdateCombat(double delta);
  void ResetBasic() { m_basicCombo = 0; } ///< @brief Resets the basic attack combo to zero
  void ResetSuper() { m_superCombo = 0; } ///< @brief Resets the super attack combo to zero
 
  std::unique_ptr<Collision::BoxCollider> m_attackCollider = nullptr;
  /**
   * @brief Sets the Attack Collider information
   * Wrapper for the JSON parsing of objects, this is used for not repeating the same code 4 times
   * @param damage Damage of the attack
   * @param size Size of the collider
   * @param offset Offset of the collider
   */
  void SetCollider(float damage, glm::vec3 size, glm::vec2 offset) const;

  // Structs with info for all the moves
  std::vector<Combat::Move> m_basicDefault;
  std::vector<Combat::Move> m_basicAir;
  std::vector<Combat::Move> m_superDefault;
  std::vector<Combat::Move> m_superAir;

  // TODO: I need animation callbacks for this pookie 😘
  bool m_isIdle = true;   ///< @brief Returns false if player is currently doing an animation (avoids spammability)
  bool m_inCombo = false; ///< @brief This stores whether the character can currently perform a combo or not

  unsigned char m_basicCombo = 0;  ///< @brief Combo status for default attack
  unsigned char m_superCombo = 0;  ///< @brief Combo status for super attack
 
  double m_hitTimer = 0.0f;
  double m_restartTime = 1.6f;
#pragma endregion

};

}
