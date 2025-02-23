#include "Character.hpp"
#include <string>
#include "Collision/OneHitCollider.hpp"
#include "Factory.hpp"
#include "GameManager.hpp"
#include "Polygon.hpp"
#include "Scene.hpp"
#include "core.hpp"
#include "glm/fwd.hpp"

#define ATTACK_ERRORS
#define ATTACK_DEBUG

namespace Sigma {

Character::~Character() = default;

void Character::Init() {
  Damageable::Init();

  // Create anim component
  m_animComp = std::make_unique<Animation::AnimationComponent>(this);
  m_animComp->SetOnAnimationEnd([this](std::string animName) { CurrentAnimationEnd(animName); });

  // Basic hit callbacks
  m_animComp->AddCallback("BasicHit", [this](std::string animName, unsigned short frame, bool loop)
                          { OnBasicHit(animName, frame, loop); });

  // Super hit callbacks
  m_animComp->AddCallback("SuperHit", [this](std::string animName, unsigned short frame, bool loop)
                          { OnSuperHit(animName, frame, loop); });

  // Tries to get Scene Bounds
  auto *scene = (GET_SCENE(0));
  if (scene->m_sceneBoundsPoly == nullptr)
    std::cerr << "[Character] " << GetName() << " failed to get GameScene\n";
  else
    m_sceneBoundsPoly = scene->m_sceneBoundsPoly;

  // Json Serialization logic
  if (!m_jsonPath.empty())
    Serialize();
#ifdef ATTACK_ERRORS
  else
    std::cerr << "[Character] No json found for " << GetName() << ". Using default values.\n";
#endif

  // Create collider
  m_attackCollider = GET_FACTORY->CreateObject<Collision::OneHitCollider>("Attack Collider");
  m_attackCollider->GetCollider()->enabled = false;
}

void Character::Start() {
  Damageable::Start();
  j.clear();
}

void Character::Update(double delta) {
  Damageable::Update(delta);

  Character::UpdateMovement(delta);
  UpdateCombat(delta);


  m_animComp->Update(delta);
}
void Character::Destroy() {
  Damageable::Destroy();
  if (m_attackCollider != nullptr)
    GET_FACTORY->DestroyObject(m_attackCollider);
}
void Character::DebugWindow() {
  Damageable::DebugWindow();

  if (ImGui::CollapsingHeader("Character")) {

    ImGui::DragFloat2("Current velocity", &velocity.x);
    ImGui::DragFloat("Max speed", &maxSpeed);
    ImGui::DragFloat("Acceleration rate", &accelerationRate);
    ImGui::DragFloat("Dash velocity", &dashVel);
    ImGui::DragFloat("Dash Time", &dashTime);
    ImGui::DragFloat("Dash Cooldown", &dashCool);
    ImGui::DragFloat("Friction", &friction);
  }
}

void Character::OnDamage(const Damage::DamageEvent &e) {
  Damageable::OnDamage(e);

  // Hit feedback anim
  if (e.GetOther() != this) {
    m_currentComboAnimName = "Hit1";
    m_animComp->SetCurrentAnim("Hit1");
    m_isIdle = false;
    // THIS IS WHERE THE OTHER HITS YOU
    glm::vec2 knockback = e.GetKnockbackAmount();
    knockback.x *= e.GetOther()->transform.relativeScale.x;
    TakeKnockback(knockback);
  }
}

void Character::TakeKnockback(glm::vec2 knockback) {
  if ((knockback.x == 0 && knockback.y == 0) || isJumping) {
    return;
  }
  velocity.x = knockback.x;
  velocity.y = knockback.y;
  //FIXME re-add jump (it never ends jump, knockback only happens once)
  isJumping = true;
  m_movementYFloor = transform.position.y;
}

glm::mat3 *Character::GetTextureTransform() {
  auto mtx = m_animComp->GetTextureMatrix();
  m_tMtx = glm::FromAEX(mtx);
  return &m_tMtx;
}

#pragma region Serialization
/**
 * @brief Helper function to load a combo
 *
 * @param combo Combo vector to save info to
 * @param j The json file
 * @param jsonKey The combo key to search on the json
 */
void LoadCombo(std::vector<Combat::Move> *combo, json_t j, const std::string &jsonKey) {
  combo->resize(j[jsonKey].size());
  for (int i = 0; i < j[jsonKey].size(); i++) {
    auto move = j[jsonKey][i];

    // combo->operator[]() is diabolical -x
    combo->operator[](i).type = Combat::GetMoveType(move["type"]);
    combo->operator[](i).damage = move["damage"];
    combo->operator[](i).knockback.x = move["knockback"]["x"];
    combo->operator[](i).knockback.y = move["knockback"]["y"];
    combo->operator[](i).colliderOffset = { move["colliderOffset"]["x"], move["colliderOffset"]["y"] };
    combo->operator[](i).colliderSize = { move["colliderSize"]["x"], move["colliderSize"]["y"], move["colliderSize"]["z"] };
    combo->operator[](i).animationName = move["animationName"];
  }
}

void Character::Serialize() {

  std::ifstream file(m_jsonPath);
  if (!file.is_open()) {
    std::cout << "[InputSystem] failed to open JSON file " << m_jsonPath << '\n';
    return;
  }
  j = json::parse(file);

  // Load character variables
  maxSpeed = j["maxSpeed"];
  accelerationRate = j["accelerationRate"];
  friction = j["friction"];
  SetMaxHealth(j["maxHealth"]);

  LoadCombo(&m_basicDefault, j, "basicCombo");
  LoadCombo(&m_basicAir, j, "basicAirCombo");
  LoadCombo(&m_superDefault, j, "superCombo");
  LoadCombo(&m_superAir, j, "superAirCombo");

  // Checks
#ifdef ATTACK_ERRORS
  if (m_basicDefault.size() != m_basicAir.size())
    std::cout << "[Attack] Basic attack does not match sizes, Default is " << m_basicDefault.size() << " and Air is "
              << m_basicAir.size() << "\n";
  if (m_superDefault.size() != m_superAir.size())
    std::cout << "[Attack] Super attack does not match sizes, Default is " << m_superDefault.size() << " and Air is "
              << m_superAir.size() << "\n";
#endif
}
#pragma endregion

#pragma region MovementSystem
void Character::Move(glm::vec2 direction) {

// This damping makes it feel better -x
  float yDamp = 0.78f;
  direction.y *= yDamp;

  if (!isDashing) {
    velocity.x += direction.x * (accelerationRate);
    velocity.y += direction.y * (accelerationRate);

    // Clamp the speed while maintaining direction
    float speed = glm::length(velocity);
    if (speed > maxSpeed)
      velocity = glm::normalize(velocity) * maxSpeed;

  } else {
      velocity = glm::normalize(velocity) * dashVel;
      if (abs(velocity.x)<= 0.01f) {velocity.y *= yDamp;}
  }
}

void Character::Dash() {
  if (!isDashing && dashTimer >= dashCool) {
    isDashing = true;
    oldVelocity = velocity;
    dashTimer = 0.f;
    // Break combo cuz if not it could crash the game -d
  }
}

void Character::UpdateMovement(double delta) {

  if (!isDashing) {
    // Apply deceleration when no input is given in X axis
    if (std::abs(velocity.x) > 0.01f) {
      if (velocity.x > 0) {
        velocity.x -= friction * delta;
        if (velocity.x < 0)
          velocity.x = 0;
        // glm::max(velocity.x, 0.0f);
      } else {
        velocity.x += friction * delta;
        if (velocity.x > 0)
          velocity.x = 0;
        // glm::min(velocity.x, 0.0f);
      }
    }

    // Apply deceleration when no input is given in Y axis
    if (std::abs(velocity.y) > 0.01f) {
      if (velocity.y > 0) {
        velocity.y -= friction * delta;
        if (velocity.y < 0)
          velocity.y = 0;
        // glm::max(velocity.y, 0.0f);
      } else {
        velocity.y += friction * delta;
        if (velocity.y > 0)
          velocity.y = 0;
        // glm::min(velocity.y, 0.0f);
      }
    }
  }

  // Calculate if in bounds
  if (m_sceneBoundsPoly != nullptr) {
    glm::vec2 newPos = transform.position;

    newPos.x += velocity.x * delta;
    if (!m_sceneBoundsPoly->IsPointInside(newPos)) {
      velocity.x = 0.0f;
    }

    newPos.y += velocity.y * delta;
    if (!m_sceneBoundsPoly->IsPointInside(newPos)) {
      velocity.y = 0.0f;
    }
  }

  // Update position
  transform.position.x += velocity.x * delta;
  transform.position.y += velocity.y * delta;

  // Dash timing
  dashTimer += delta;

  if (isDashing && dashTimer >= dashTime) {
    velocity = oldVelocity;
    isDashing = false;
    dashTimer = 0.0f;
  }

  // Update Z
  transform.position.z = -transform.position.y;
}
#pragma endregion

#pragma region Combat
void Character::UpdateCombat(double delta) {
  m_hitTimer += delta;

  // Handles reseting combo after not attacking for a while -x
  if (m_hitTimer > m_restartTime && m_inCombo) {
#ifdef ATTACK_DEBUG
    std::cout << "[Attack] Combo restarted\n";
#endif
    ResetBasic();
    ResetSuper();

    m_hitTimer = 0.0f;
    m_inCombo = false;
  }
}

void Character::CurrentAnimationEnd(std::string &animName) {
  if (animName == m_currentComboAnimName) {
    m_isIdle = true;
    m_animComp->SetCurrentAnim("Idle");
  }
}

void Character::BasicAttack() {
  if (!m_isIdle) return;
  if (isDashing) return;

  m_inCombo = true;
  m_isIdle = false;
  ResetSuper();

  m_hitTimer = 0;

  // The game crashes when the player is jumping while in the middle of a combo -d
  // FIXME: This is a temporary fix -d

  auto move = m_basicDefault[m_basicCombo];
  m_currentComboAnimName = move.animationName;
  m_animComp->SetCurrentAnim(move.animationName);
 
  #ifdef ATTACK_DEBUG
  std::cout << "[Attack] " << move.animationName << "\n";
  #endif

  m_basicCombo++;

  // I'm using only the count from the default variant since both should have the same number -x
  if (m_basicCombo >= m_basicDefault.size()) {
    ResetBasic();
    OnFullComboPerformed();
  }
}

void Character::SuperAttack() {
  if (!m_isIdle) return;
  if (isDashing) return;

  m_inCombo = true;
  m_isIdle = false;
  ResetBasic();

  m_hitTimer = 0;

  auto move = m_superDefault[m_basicCombo];
  m_currentComboAnimName = move.animationName;
  m_animComp->SetCurrentAnim(move.animationName);
 
  #ifdef ATTACK_DEBUG
  std::cout << "[Attack] " << move.animationName << "\n";
  #endif

  if (m_superCombo >= m_superDefault.size()) {
    ResetSuper();
    OnFullComboPerformed();
  }
}

void Character::SetCollider(const float damage,const glm::vec2 knockback, const glm::vec3 size, const glm::vec2 offset) {
  float side = std::clamp(transform.relativeScale.x, -1.0f, 1.0f);
  glm::vec3 position = {transform.position.x + offset.x * side, transform.position.y + offset.y, transform.position.z};
  m_attackCollider->Do(position, size, damage,knockback, this, true);
}

// The callbacks could be on only one by doing string.contains() but I feel it's better to have them separated onto two
// -x

// BASIC HIT
void Character::OnBasicHit(std::string &animName, unsigned short frame, bool loop) {
  // Sets the current move to jumping or not according if the player isJumping or not -x
  auto move = isJumping ? m_basicAir[m_basicCombo] : m_basicDefault[m_basicCombo];
  m_superCombo++;
  SetCollider(move.damage,move.knockback, move.colliderSize, move.colliderOffset);
}

// SUPER HIT
void Character::OnSuperHit(std::string &animName, unsigned short frame, bool loop) {
  // Sets the current move to jumping or not according if the player isJumping or not -x
  auto move = isJumping? m_superAir[m_superCombo] : m_superDefault[m_superCombo];
  SetCollider(move.damage,move.knockback, move.colliderSize, move.colliderOffset);
}
#pragma endregion

} // namespace Sigma
