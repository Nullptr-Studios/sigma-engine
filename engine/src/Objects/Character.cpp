#include "Character.hpp"
#include "json.hpp"

namespace Sigma {
using json = nlohmann::json;

Character::~Character() = default;

void Character::Init() {
  Actor::Init();

  m_animComp = std::make_unique<Animation::AnimationComponent>();
}

void Character::Start() {
  Actor::Start();

  if (m_jsonPath != "") Serialize();
  else std::cout << "No json found for " << GetName() << ". Using default values.";
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
  json j = json::parse(file);

  // Load character variables
  maxSpeed = j["maxSpeed"];
  accelerationRate = j["accelerationRate"];
  jumpVel = j["jumpVel"];

  LoadCombo(&m_basicDefault, j, "basicCombo");
  LoadCombo(&m_basicAir, j, "basicAirCombo");
  LoadCombo(&m_superDefault, j, "superCombo");
  LoadCombo(&m_superAir, j, "superAirCombo");
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
    std::cout << "Combo restarted\n";
    ResetBasic();
    ResetSuper();

    m_hitTimer = 0.0f;
    m_inCombo = false;
  }
}

void Character::BasicAttack() {
  if (!m_isIdle) return;

  m_basicCombo++;
  m_hitTimer = 0;
  m_inCombo = true;
  ResetSuper();
 
  if (!isJumping) {
    std::cout << "Basic Attack " << (unsigned int)m_basicCombo << "\n";
  } else {
    std::cout << "Basic AIR Attack " << (unsigned int)m_basicCombo << "\n";
  }

  if (m_basicCombo >= m_basicMoveCount) ResetBasic();
}

void Character::SuperAttack() {
  if (!m_isIdle) return;

  m_superCombo++;
  m_hitTimer = 0;
  m_inCombo = true;
  ResetBasic();

  if (!isJumping) {
    std::cout << "Super Attack " << (unsigned int)m_superCombo << "\n";
  } else {
    std::cout << "Super AIR Attack " << (unsigned int)m_superCombo << "\n";
  }

  if (m_superCombo >= m_superMoveCount) ResetBasic();
}

#pragma endregion

} // namespace FNFE
