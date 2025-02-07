#include "Character.hpp"
#include "AnimationSystem/AnimationComponent.hpp"

namespace Sigma {

void Character::Init() {
  Actor::Init();

  //TODO: fix animation stuff (Acess Violation)
    //if (m_animComp == nullptr)
  //m_animComp = new ANIMATION::AnimationComponent();

}
void Character::OnDamage(Damage::DamageEvent &e) {
  float characterHealth = m_health - e.GetDamageAmount();
  SetHealth(characterHealth);
  if (characterHealth <= 0) { m_isAlive = false; }
}


glm::mat3& Character::GetTextureTransform() {
  if (m_animComp == nullptr) {
    m_tMtx = glm::mat3(1.0f);
    return m_tMtx;
  }
  return m_animComp->GetTextureMatrix();
}

}
