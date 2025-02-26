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

float Character::m_bounceDampener = 0.75;
Character::~Character() = default;

void Character::Init() {
  Damageable::Init();

  // Create anim component
  m_animComp = std::make_unique<Animation::AnimationComponent>(this);
  m_animComp->SetOnAnimationEnd([this](std::string animName) { CurrentAnimationEnd(animName); });

  // Basic hit callbacks
  m_animComp->AddCallback("DoHit", [this](std::string animName, unsigned short frame, bool loop)
                          { DoHit(animName, frame, loop); });

  /*m_animComp->AddCallback("Grab", [this](std::string animName, unsigned short frame, bool loop)
                          { OnGrab(animName, frame, loop); });

  m_animComp->AddCallback("Throw", [this](std::string animName, unsigned short frame, bool loop)
                          { ThrowGrabbedCharacter(animName, frame, loop); });*/
  

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
    ImGui::DragFloat("Jump velocity", &jumpVel);
    ImGui::DragFloat("Friction", &friction);
    ImGui::DragFloat("Gravity", &gravity);
    ImGui::DragFloat("Terminal velocity", &terminalVel);
    ImGui::DragFloat("Bounce Dampener", &m_bounceDampener);
  }

  if (ImGui::CollapsingHeader("Moveset")) {
    ImGui::Checkbox("Draw debug hit collider", &m_drawDebugCollider);
  }
}

void Character::OnDamage(const Damage::DamageEvent &e) {
  Damageable::OnDamage(e);

  if (e.GetOther() == this)
    return;

  if (e.GetDamageType() == Damage::DAMAGE) {
    // Hit feedback anim
    // TODO: Change to support more animations -d
    m_currentComboAnimName = "Hit1";
    m_animComp->SetCurrentAnim("Hit1");
    m_isIdle = false;
  }else if (e.GetDamageType() == Damage::GRAB){ // Grab
    // m_currentComboAnimName = "Grabbed";
    m_animComp->SetCurrentAnim("Grabbed");
    
    auto player = dynamic_cast<Character*>(e.GetOther());
    if (player != nullptr) {
      if (!player->SetGrabbedObject(this)) {
        m_isIdle = true;
        return;
      }
    }

    m_isIdle = false;
  }else { //TODO: throw

    std::cout << "Thrown\n";
    m_currentComboAnimName = "Hit1";
    m_animComp->SetCurrentAnim("Hit1");
    m_isIdle = false;

  }

  // THIS IS WHERE THE OTHER HITS YOU
  glm::vec2 knockback = e.GetKnockbackAmount();
  knockback.x *= e.GetOther()->transform.relativeScale.x;
  TakeKnockback(knockback);
  
}

void Character::TakeKnockback(glm::vec2 knockback) {
  if ((knockback.x == 0 && knockback.y == 0)|| isInAir) {
    return;
  }
  velocity.x = knockback.x;
  velocity.y = knockback.y;
  isInAir = true;
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
  jumpVel = j["jumpVel"];
  friction = j["friction"];
  SetMaxHealth(j["maxHealth"]);

  LoadCombo(&m_basicDefault, j, "basicCombo");
  //LoadCombo(&m_basicAir, j, "basicAirCombo");
  LoadCombo(&m_superDefault, j, "superCombo");
  //LoadCombo(&m_superAir, j, "superAirCombo");

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
  // WTF is this? -d
  direction.y *= 0.78f;

  if (!isInAir) {
    velocity.x += (direction.x * (accelerationRate)) * AEGetFrameRate();
    velocity.y += (direction.y * (accelerationRate)) * AEGetFrameRate();

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
  if (!isInAir) {
    velocity.y = jumpVel;
    isInAir = true;
    m_movementYFloor = transform.position.y;

    // Break combo cuz if not it could crash the game -d
  }
}

void Character::UpdateMovement(double delta) {
  // Apply gravity
  if (isInAir) {
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
  if (!isInAir) {
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
    glm::vec2 newPos = !isInAir ? transform.position : glm::vec2(transform.position.x, m_movementYFloor);

    newPos.x += velocity.x * delta;
    if (!m_sceneBoundsPoly->IsPointInside(newPos)) {
      if (isInAir) {
        velocity.x *= -m_bounceDampener;
      } else {
        velocity.x = 0.0f;
      }
    }

    if (!isInAir) {
      newPos = transform.position;

      newPos.y += velocity.y * delta;
      if (!m_sceneBoundsPoly->IsPointInside(newPos)) {
        velocity.y = 0.0f;
      }
    }
  }

  // Update position
  transform.position.x += velocity.x * delta;
  transform.position.y += velocity.y * delta;

  // Ground collision
  if (isInAir && transform.position.y <= m_movementYFloor) {
    transform.position.y = m_movementYFloor;
    velocity.y = 0;
    isInAir = false;
  }

  // Update Z
  if (!isInAir)
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
    
    m_isIdle = true;
  }
}

void Character::CurrentAnimationEnd(std::string &animName) {
  // don´t stop the grab if the animation ends
  if (animName == m_currentComboAnimName && m_currentMove.type != Combat::MoveType::GRB) {
    m_isIdle = true;
    m_animComp->SetCurrentAnim("Idle");
  }else if (m_currentMove.type == Combat::MoveType::GRB) {
    
  }
}

void Character::BasicAttack() {
  if (!m_isIdle && m_currentMove.type != Combat::MoveType::GRB)
    return;

  m_inCombo = true;
  m_isIdle = false;
  ResetSuper();

  m_hitTimer = 0;

  // The game crashes when the player is jumping while in the middle of a combo -d
  // FIXME: This is a temporary fix -d
  if (!isInAir) {
    auto move = m_basicDefault[m_basicCombo];
    m_currentMove = move;
    m_currentComboAnimName = move.animationName;
    m_animComp->SetCurrentAnim(move.animationName);

#ifdef ATTACK_DEBUG
    std::cout << "[Attack] " << move.animationName << "\n";
#endif
  }

  m_basicCombo++;

  // I'm using only the count from the default variant since both should have the same number -x
  if (m_basicCombo >= m_basicDefault.size()) {
    ResetBasic();
    OnFullComboPerformed();
  }
}

void Character::SuperAttack() {
  if (!m_isIdle)
    return;

  m_inCombo = true;
  m_isIdle = false;
  ResetBasic();

  m_hitTimer = 0;

  if (!isInAir) {
    auto move = m_superDefault[m_basicCombo];
    m_currentComboAnimName = move.animationName;
    m_animComp->SetCurrentAnim(move.animationName);

#ifdef ATTACK_DEBUG
    std::cout << "[Attack] " << move.animationName << "\n";
#endif
  }

  m_superCombo++;

  if (m_superCombo >= m_superDefault.size()) {
    ResetSuper();
    OnFullComboPerformed();
  }
}

void Character::SetCollider(const float damage, const glm::vec3 size, const glm::vec2 offset, Damage::DamageType type, glm::vec2 knockback) {
  float side = std::clamp(transform.relativeScale.x, -1.0f, 1.0f);
  glm::vec3 position = {transform.position.x + offset.x * side, transform.position.y + offset.y, transform.position.z};
  m_attackCollider->Do(position, size, damage, this, type, knockback,true);
}

// The callbacks could be on only one by doing string.contains() but I feel it's better to have them separated onto two
// -x

// TODO: Expand this -d
void Character::DoHit(std::string &animName, unsigned short frame, bool loop) {

  auto move = m_currentMove;
  switch (m_currentMove.type) {
    case Combat::DMG:
      SetCollider(move.damage, move.colliderSize, move.colliderOffset, Damage::DAMAGE, move.knockback);
    break;
    case Combat::GRB:
      m_grabbedCharacter = nullptr;
      SetCollider(move.damage, move.colliderSize, move.colliderOffset, Damage::GRAB, move.knockback);
    break;
    case Combat::THR:
      if (m_grabbedCharacter != nullptr) {
        m_grabbedCharacter->OnDamage(Damage::DamageEvent(m_grabbedCharacter->GetId(), this,Collision::DAMAGE, move.damage, move.knockback, Damage::THROW));
        m_grabbedCharacter = nullptr;
      }
    break;
    
    default:
      std::cerr << "[Attack] Move type not found\n";
    break;
  }
}

/*// BASIC HIT
void Character::OnNormalHit(std::string &animName, unsigned short frame, bool loop) {
  // Sets the current move to jumping or not according if the player isInAir or not -x
  auto move = m_currentMove;
  SetCollider(move.damage, move.colliderSize, move.colliderOffset, Damage::DAMAGE, move.knockback);
}

// Grab
void Character::OnGrab(std::string &animName, unsigned short frame, bool loop) {
  auto move = m_currentMove;

  SetCollider(move.damage, move.colliderSize, move.colliderOffset, Damage::GRAB, move.knockback);
}

// Throw
void Character::ThrowGrabbedCharacter(std::string &animName, unsigned short frame, bool loop) {
  auto move = m_currentMove;
  if (m_grabbedCharacter != nullptr) {
    m_grabbedCharacter->OnDamage(Damage::DamageEvent(m_grabbedCharacter->GetId(), this,Collision::DAMAGE, move.damage, move.knockback, Damage::THROW));
    m_grabbedCharacter = nullptr;
  }
}*/
#pragma endregion

} // namespace Sigma
