#include "Character.hpp"

namespace FNFE {

Character::~Character() {
  delete m_animComp;
}

void Character::Init() {
  Actor::Init();

  if (m_animComp == nullptr)
    m_animComp = new ANIMATION::AnimationComponent();

}

glm::mat3& Character::GetTextureTransform() {
  return m_animComp->GetTextureMatrix();
}

}
