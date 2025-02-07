#include "Camera.hpp"
#include "GlmAlphaTools.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>

namespace Sigma {

void Camera::Start() {
  Object::Start();

  glm::vec2 viewport;
  AEGfxSetFullscreen(false);
  AEGfxGetViewRectangle(&viewport.x, &viewport.y);
  m_ratio = viewport.x / viewport.y;

  m_oldTransform = transform;
  UpdateMatrix();
  // hi
}

void Camera::Update(double deltaTime) {
  Object::Update(deltaTime);

  if (m_oldTransform.position != transform.position || m_oldTransform.rotation != transform.rotation) {
    UpdateMatrix();
    m_oldTransform = transform;
  }

  // Check for rescaling
  RECT rect;
  glm::vec2 viewport;
  AEGfxGetViewRectangle(&viewport.x, &viewport.y);
  m_ratio = viewport.x / viewport.y;
  GetClientRect(AEGetWindowHandler(), &rect);
  glm::vec2 client = glm::vec2(rect.right - rect.left, rect.bottom - rect.top);
  if (viewport != client) {
    AEGfxSetViewRectangle(client.x, client.y);
    UpdateMatrix();
  }
  // DEBUG
  AEGfxRect(0,0,0,viewport.x-1,viewport.y-1,AE_COLORS_BLUE);
  AEGfxRect(0,0,0,viewport.x*.5,viewport.y *.5,AE_COLORS_BLUE);
  auto mousedata = AEGetMouseData().position.ToVec3();
  auto mousepos = glm::FromAEX(mousedata)*.01f;
  int scale = 10;
  mousepos.x = std::clamp(mousepos.x,-1.0f*scale,1.0f*scale);
  mousepos.y = std::clamp(mousepos.y,-1.0f*scale,1.0f*scale);
  LerpToPosition(mousepos,.3*AEGetFrameTime());
  
}

void Camera::UpdateMatrix() {
  // View Space
  m_viewMatrix = glm::mat4(1.0f);
  m_viewMatrix = glm::translate(m_viewMatrix, -transform.position);
  m_viewMatrix = glm::rotate(m_viewMatrix, -transform.rotation, glm::vec3(0, 0, 1));

  // Clip Space
  glm::vec2 viewport;
  AEGfxGetViewRectangle(&viewport.x, &viewport.y);
  m_clipMatrix = glm::ortho(-viewport.x/2, viewport.x/2 , -viewport.y/2,  viewport.y/2, m_near, m_far);
}

void Camera::LerpToPosition(glm::vec3 position, float delta) {
  transform.position = glm::lerp(transform.position, position, delta);
}


} // namespace Sigma
#undef GLM_USE_EXPERIMENTAL
