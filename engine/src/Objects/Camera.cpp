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

glm::vec2 Camera::WorldToScreen(glm::vec2 worldPos) const {
  glm::vec2 cameraPos = {transform.position.x,transform.position.y};
  return worldPos - cameraPos;
}

glm::vec2 Camera::ScreenToWorld(glm::vec2 screenPos) const {
  glm::vec2 cameraPos = {transform.position.x,transform.position.y};
  return screenPos + cameraPos;
}

void Camera::LerpToPosition(glm::vec3 position, float delta) {
  glm::vec2 viewport;
  AEGfxGetViewRectangle(&viewport.x, &viewport.y);
  glm::vec2 screenPos = WorldToScreen({position.x,position.y});
  screenPos = {screenPos.x/viewport.x*2,screenPos.y/viewport.y*2};// What region of the screen the object is at 1,1 being top right -1,-1 bottom left
  // std::ostringstream oss; // DEBUG
  // oss << screenPos.x << " " << screenPos.y << "\n";
  if (-.25f < screenPos.x && screenPos.x < .25f && -.25f < screenPos.y && screenPos.y < .25f) {
    transform.position = glm::lerp(transform.position, position, delta);
    // oss << "a";
  } else if (-.75f < screenPos.x && screenPos.x < .75f && -.75f < screenPos.y && screenPos.y < .75f) {
    transform.position = glm::lerp(transform.position, position, 3.0f*delta);
    // oss << "b";
  } else {
    transform.position = glm::lerp(transform.position, position, 10.0f * delta);
    // oss << "c";
  }
  // AEGfxPrint(10, 240, 0xFFFFFFFF, oss.str().c_str());
}


} // namespace Sigma
#undef GLM_USE_EXPERIMENTAL
