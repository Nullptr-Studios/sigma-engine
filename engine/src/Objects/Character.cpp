#include "Character.hpp"

namespace FNFE {

void Character::Init() {
  Actor::Init();

  if (m_animComp == nullptr)
    m_animComp = new ANIMATION::AnimationComponent();

}
void Character::Start() { Actor::Start(); }
void Character::Update(double delta) { Actor::Update(delta); }
void Character::Destroy() { Actor::Destroy(); }

glm::mat3& Character::GetTextureTransform() {
  return m_animComp->GetTextureMatrix();
}

}
