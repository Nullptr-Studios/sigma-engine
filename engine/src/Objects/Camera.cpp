#include "Camera.hpp"

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
  m_viewMatrix = glm::scale(m_viewMatrix, glm::vec3(1.0f));

  // Clip Space
  glm::vec2 viewport;
  AEGfxGetViewRectangle(&viewport.x, &viewport.y);
  m_clipMatrix = glm::ortho(-viewport.x/2, viewport.x/2 , -viewport.y/2,  viewport.y/2, m_near, m_far);
}

} // namespace Sigma
