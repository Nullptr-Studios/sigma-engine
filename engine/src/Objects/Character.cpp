#include "Character.hpp"

namespace FNFE {

void Character::Init() {
  Actor::Init();

  //TODO: fix animation stuff (Acess Violation)
    //if (m_animComp == nullptr)
  //m_animComp = new ANIMATION::AnimationComponent();

}

glm::mat3& Character::GetTextureTransform() {
  return m_animComp->GetTextureMatrix();
}

}
