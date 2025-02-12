#include "Character.hpp"
#include <string>
#include "Collision/OneHitCollider.hpp"
#include "GameManager.hpp"
#include "GameScene.hpp"
#include "Polygon.hpp"
#include "core.hpp"
#include "Factory.hpp"
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
  m_animComp->AddCallback("OnBasicHit", [this](std::string animName, unsigned short frame, bool loop)
    { OnBasicHit(animName, frame, loop); }
  );

  // Super hit callbacks
  m_animComp->AddCallback("OnSuperHit", [this](std::string animName, unsigned short frame, bool loop)
    { OnSuperHit(animName, frame, loop); }
  );

  // Tries to get Scene Bounds
  auto* scene = dynamic_cast<GameScene*>(GET_SCENE);
  if (scene == nullptr)
    std::cerr << "[Character] " << GetName() << " failed to get GameScene\n";
  else
    m_sceneBoundsPoly = scene->GetSceneBoundsPoly();

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
}

void Character::Update(double delta) {
  Damageable::Update(delta);
  
  Character::UpdateMovement(delta);
  UpdateCombat(delta);

  
  m_animComp->Update(delta);
}

// TODO: character damage logic
void Character::OnDamage(const Damage::DamageEvent &e) {
  Damageable::OnDamage(e);
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
void LoadCombo(std::vector<Combat::Move>* combo, json_t j, const std::string& jsonKey) {
  combo->resize(j[jsonKey].size());
  for (int i = 0; i < j[jsonKey].size(); i++) {
    auto move = j[jsonKey][i];

    // combo->operator[]() is diabolical -x
    combo->operator[](i).type = Combat::GetMoveType(move["type"]);
    combo->operator[](i).damage = move["damage"];
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
  jumpVel = j["jumpVel"];

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

  if (!isJumping) {
    velocity.x += direction.x * (accelerationRate * AEGetFrameRate());
    velocity.y += direction.y * (accelerationRate * AEGetFrameRate());

    // Clamp the speed while maintaining direction
    float speed = glm::length(velocity);
    if (speed > maxSpeed)
      velocity = glm::normalize(velocity) * maxSpeed;
  } else {
    velocity.x += direction.x * (accelerationRate * AEGetFrameRate());
    velocity.x = glm::clamp(velocity.x, -maxSpeed, maxSpeed);
  }
}

void Character::Jump() {
  if (!isJumping) {
    velocity.y = jumpVel * AEGetFrameRate();
    isJumping = true;
    m_movementYFloor = transform.position.y;
  }
}

void Character::UpdateMovement(double delta) {
  // Apply gravity
  if (isJumping) {
    velocity.y += gravity * delta;
    velocity.y = glm::clamp(velocity.y, -terminalVel, terminalVel);
  }

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
  if (!isJumping) {
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
    glm::vec2 newPos = !isJumping ? transform.position : glm::vec2(transform.position.x, m_movementYFloor);

    newPos.x += velocity.x * delta;
    if (!m_sceneBoundsPoly->isPointInside(newPos)) {
      velocity.x = 0.0f;
    }

    if (!isJumping) {
      newPos = transform.position;

      newPos.y += velocity.y * delta;
      if (!m_sceneBoundsPoly->isPointInside(newPos)) {
        velocity.y = 0.0f;
      }
    }
  }

  // Update position
  transform.position.x += velocity.x * delta;
  transform.position.y += velocity.y * delta;

  // Ground collision
  if (isJumping && transform.position.y <= m_movementYFloor) {
    transform.position.y = m_movementYFloor;
    velocity.y = 0;
    isJumping = false;
  }

  // Update Z
  if (!isJumping) transform.position.z = -transform.position.y;
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

void Character::CurrentAnimationEnd(std::string& animName) { m_isIdle = true; }

void Character::BasicAttack() {
  if (!m_isIdle) return;

  m_inCombo = true;
  m_isIdle = false;
  ResetSuper();

  m_hitTimer = 0;
  m_basicCombo++;

  if (!isJumping) {
    auto move = m_basicDefault[m_basicCombo - 1];
    m_animComp->SetCurrentAnim(move.animationName);
 
    #ifdef ATTACK_DEBUG
      std::cout << "[Attack] " << move.animationName << "\n";
    #endif
  } else {
    auto move = m_basicAir[m_basicCombo - 1];
    m_animComp->SetCurrentAnim(move.animationName);

    #ifdef ATTACK_DEBUG
      std::cout << "[Attack] " << move.animationName << "\n";
    #endif
  }

  // I'm using only the count from the default variant since both should have the same number -x
  if (m_basicCombo >= m_basicDefault.size())
    ResetBasic();
}

void Character::SuperAttack() {
  if (!m_isIdle) return;

  m_inCombo = true;
  m_isIdle = false;
  ResetBasic();

  m_hitTimer = 0;
  m_superCombo++;

  if (!isJumping) {
    auto move = m_basicDefault[m_basicCombo - 1];
    m_animComp->SetCurrentAnim(move.animationName);
 
    #ifdef ATTACK_DEBUG
    std::cout << "[Attack] " << move.animationName << "\n";
    #endif
  } else {
    auto move = m_basicAir[m_basicCombo - 1];
    m_animComp->SetCurrentAnim(move.animationName);

    #ifdef ATTACK_DEBUG
    std::cout << "[Attack] " << move.animationName << "\n";
    #endif
  }

  if (m_superCombo >= m_superDefault.size())
    ResetSuper();
}

void Character::SetCollider(const float damage, const glm::vec3 size, const glm::vec2 offset) {
  float side = std::clamp(transform.relativeScale.x, -1.0f, 1.0f);
  glm::vec3 position = {transform.position.x + offset.x * side, transform.position.y + offset.y, transform.position.z};
  m_attackCollider->Do(position, size, damage, this, true);
}

// The callbacks could be on only one by doing string.contains() but I feel it's better to have them separated onto two -x

// BASIC HIT
void Character::OnBasicHit(std::string& animName, unsigned short frame, bool loop) {
  // Sets the current move to jumping or not according if the player isJumping or not -x
  auto move = isJumping? m_basicAir[m_basicCombo - 1] : m_basicDefault[m_basicCombo - 1];
  SetCollider(move.damage, move.colliderSize, move.colliderOffset);
}

//SUPER HIT
void Character::OnSuperHit(std::string& animName, unsigned short frame, bool loop) {
  // Sets the current move to jumping or not according if the player isJumping or not -x
  auto move = isJumping? m_superAir[m_superCombo - 1] : m_superDefault[m_superCombo - 1];
  SetCollider(move.damage, move.colliderSize, move.colliderOffset);
}
#pragma endregion

} // namespace Sigma
