#include "Actor.hpp"

#include "AnimationSystem/AnimationComponent.hpp"
#include "Camera.hpp"
#include "Collision/Collider.hpp"
#include "Controller/CameraController.hpp"
#include "Factory.hpp"
#include "GameManager.hpp"


namespace Sigma {

void Actor::SetTexture(const char *path) {
  m_texturePath = path;
  m_texture = GET_FACTORY->LoadTexture(m_texturePath);
}

void Actor::SetTextureTransform(const glm::mat3 &newTexMtx) { m_tMtx = newTexMtx; }

glm::mat3 *Actor::GetTextureTransform() { return &m_tMtx; }

bool Actor::IsInViewport() const {
  auto c = GET_CAMERA->GetCurrentCamera();
  int w = AEGetSysWindowWidth();
  int h = AEGetSysWindowHeight();

  auto pos = c->transform.position;
  return transform.position.x > pos.x - w / 2 && transform.position.x < pos.x + w / 2 &&
         transform.position.y > pos.y - h / 2 && transform.position.y < pos.y + h / 2;
}

} // namespace Sigma
