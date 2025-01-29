#include "Actor.hpp"

#include "Camera.hpp"
#include "Factory.hpp"
#include "GameManager.hpp"
#include "AnimationSystem/AnimationComponent.hpp"
#include "Collision/Collider.hpp"

namespace FNFE {

// Init
void Actor::Init() {
  Object::Init();
  //
  // AEGfxTriStart();
  // AEGfxTriAdd(-0.5f, -0.5f, AE_COLORS_WHITE, 0.0f, 0.0f, -0.5f, 0.5f, AE_COLORS_WHITE, 0.0f, 1.0f, 0.5f, -0.5f,
  //             AE_COLORS_WHITE, 1.0f, 0.0f);
  // AEGfxTriAdd(-0.5f, 0.5f, AE_COLORS_WHITE, 0.0f, 1.0f, 0.5f, 0.5f, AE_COLORS_WHITE, 1.0f, 1.0f, 0.5f, -0.5f,
  //             AE_COLORS_WHITE, 1.0f, 0.0f);
  //
  // m_triangleList = AEGfxTriEnd();
  //
  // m_texture = FNFE_FACTORY->LoadTexture(m_texturePath);

}

void Actor::Destroy() {
  Object::Destroy();
  /*FNFE_FACTORY->FreeTexture(m_texturePath);
  m_texture = nullptr;*/

}

void Actor::SetTexture(const char* path) {
  m_texturePath = path;
  m_texture = FNFE_FACTORY->LoadTexture(m_texturePath);
}

void Actor::SetTextureTransform(glm::mat3& newTexMtx)
{
  m_tMtx = newTexMtx;
}

glm::mat3& Actor::GetTextureTransform() {

  return m_tMtx;
}

bool Actor::IsInViewport()
{
  auto c = FNFE_MANAGER->GetActiveCamera();
  int w = AEGetSysWindowWidth();
  int h = AEGetSysWindowHeight();

  auto pos = c->transform.position;
  return transform.position.x > pos.x - w/2 && transform.position.x < pos.x + w/2 && transform.position.y > pos.y - h/2 && transform.position.y < pos.y + h/2;
  
}

}
