#include "Camera.hpp"

namespace FNFE {

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
  if (viewport != client)
    AEGfxSetViewRectangle(client.x, client.y);
}

void Camera::UpdateMatrix() {
  // View Space
  auto viewMatrix = glm::mat4(1.0f);
  viewMatrix = glm::translate(glm::mat4(1.0f), transform.position);
  viewMatrix = glm::rotate(m_cameraMatrix, transform.rotation, glm::vec3(0, 0, 1));

  // Clip Space
  glm::vec2 viewport;
  AEGfxGetViewRectangle(&viewport.x, &viewport.y);
  auto clipMatrix = glm::ortho(0.0f, viewport.x, viewport.y, 0.0f, m_near, m_far);
  m_cameraMatrix = clipMatrix * viewMatrix;
}

} // namespace FNFE
