#include "Character.hpp"
#include "Collision/Collider.hpp"
#define ATTACK_ERRORS
#define ATTACK_DEBUG

namespace Sigma {

Character::~Character() = default;

void Character::Init() {
  Actor::Init();

  m_animComp = std::make_unique<Animation::AnimationComponent>();
}

void Character::Start() {
  Actor::Start();

  if (!m_jsonPath.empty()) Serialize();
#ifdef ATTACK_ERRORS
  else std::cerr << "[Character] No json found for " << GetName() << ". Using default values.";
#endif

  m_attackCollider = std::make_unique<Collision::BoxCollider>(Collision::PLAYER | Collision::ENEMY, Collision::DAMAGE);
  m_attackCollider->enabled = false;
}

void Character::Update(double delta) {
  Actor::Update(delta);
  UpdateMovement(delta);
  UpdateCombat(delta);
}

/**
 * @brief Helper function to load a combo
 *
 * @param combo Combo vector to save info to
 * @param j The json file
 * @param jsonKey The combo key to search on the json
 */
void LoadCombo(std::vector<Combat::Move>* combo, json j, const std::string& jsonKey) {
  combo->resize(j[jsonKey].size());
  for (int i = 0; i < j[jsonKey].size(); i++) {
    auto move = j[jsonKey][i];

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
    std::cout << "[Attack] Basic attack does not match sizes, Default is " << m_basicDefault.size() << " and Air is " << m_basicAir.size() << "\n";
  if (m_superDefault.size() != m_superAir.size())
    std::cout << "[Attack] Super attack does not match sizes, Default is " << m_superDefault.size() << " and Air is " << m_superAir.size() << "\n";
#endif
}

glm::mat3 &Character::GetTextureTransform() {
  if (m_animComp == nullptr) {
    m_tMtx = glm::mat3(1.0f);
    return m_tMtx;
  }
  return m_animComp->GetTextureMatrix();
}

#pragma region MovementSystem

void Character::Move(glm::vec2 direction) {
  if (!isJumping) {
    velocity += direction * maxSpeed;
    velocity = glm::clamp(velocity, -maxSpeed, maxSpeed);
  }else {
    velocity.x += direction.x * maxSpeed;
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

void Character::UpdateMovement(double delta)
{
  // Apply gravity
  if (isJumping) {
    velocity.y += gravity * delta;
    velocity.y = glm::clamp(velocity.y, -terminalVel, terminalVel);
  }

  // Apply deceleration when no input is given in X axis
  if (std::abs(velocity.x) > 0.01f) {
    if (velocity.x > 0) {
      velocity.x -= friction * delta;
      glm::max(velocity.x, 0.0f); 
    }
    else
    {
      velocity.x += friction * delta;
      glm::min(velocity.x, 0.0f); 
    }
  }
 
  // Apply deceleration when no input is given in Y axis
  if (!isJumping) {
    if (std::abs(velocity.y) > 0.01f) {
      if (velocity.y > 0) {
        velocity.y -= friction * delta;
        glm::max(velocity.y, 0.0f); 
      }
      else
      {
        velocity.y += friction * delta;
        glm::min(velocity.y, 0.0f); 
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

 
  //Update Z
  if (!isJumping)
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

// TODO: Right now this is being called on the Basic Attack
// When my pookie dario has the animation system callbacks the collider should be enabled there
// This works for now but will change later on -x
void Character::SetCollider(const float damage, const glm::vec3 size, const glm::vec2 offset) const {
  m_attackCollider->box.Set(size.x/2, size.x/2, size.y/2, size.y/2, offset);
  m_attackCollider->depth = size.z;
  m_attackCollider->damage = damage;
  m_attackCollider->enabled = true;
}

void Character::BasicAttack() {
  if (!m_isIdle) return;

  m_basicCombo++;
  m_hitTimer = 0;
  m_inCombo = true;
  ResetSuper();
 
  if (!isJumping) {
    auto move = m_basicDefault[m_basicCombo - 1];
#ifdef ATTACK_DEBUG
    std::cout << "[Attack] " << move.animationName << "\n";
#endif
    SetCollider(move.damage, move.colliderSize, move.colliderOffset);
  } else {
    auto move = m_basicAir[m_basicCombo - 1];
#ifdef ATTACK_DEBUG
    std::cout << "[Attack] " << move.animationName << "\n";
#endif
    SetCollider(move.damage, move.colliderSize, move.colliderOffset);
  }

  // I'm using only the count from the default variant since both should have the same number -x
  if (m_basicCombo >= m_basicDefault.size()) ResetBasic();
}

void Character::SuperAttack() {
  if (!m_isIdle) return;

  m_superCombo++;
  m_hitTimer = 0;
  m_inCombo = true;
  ResetBasic();

  if (!isJumping) {
    auto move = m_superDefault[m_superCombo - 1];
#ifdef ATTACK_DEBUG
    std::cout << "[Attack] " << move.animationName << "\n";
#endif
    SetCollider(move.damage, move.colliderSize, move.colliderOffset);
  } else {
    auto move = m_superAir[m_superCombo - 1];
#ifdef ATTACK_DEBUG
    std::cout << "[Attack] " <<move.animationName << "\n";
#endif
    SetCollider(move.damage, move.colliderSize, move.colliderOffset);
  }

  if (m_superCombo >= m_superDefault.size()) ResetSuper();
}

#pragma endregion

} // namespace FNFE
