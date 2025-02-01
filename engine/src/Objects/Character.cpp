#include "Character.hpp"

namespace sigma {

void Character::Init() {
  Actor::Init();

  //TODO: fix animation stuff (Acess Violation)
    //if (m_animComp == nullptr)
  //m_animComp = new ANIMATION::AnimationComponent();

}

glm::mat3& Character::GetTextureTransform() {
  if (m_animComp == nullptr) {
    m_tMtx = glm::mat3(1.0f);
    return m_tMtx;
  }
  return m_animComp->GetTextureMatrix();
}

}
