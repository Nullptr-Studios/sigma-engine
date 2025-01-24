#include "Camera.hpp"

namespace FNFE {

void Camera::Start() {
  Object::Start();

  AEVec2 viewport;
  AEGfxSetFullscreen(false);
  AEGfxGetViewRectangle(&viewport.x, &viewport.y);
  m_ratio = viewport.x / viewport.y;
  
  m_oldTransform = transform;
  UpdateMatrix();
}

void Camera::Update(double deltaTime) {
  Object::Update(deltaTime);

  if (m_oldTransform.position != transform.position || m_oldTransform.rotation != transform.rotation) {
    UpdateMatrix();
    m_oldTransform = transform;
  }

  // Check for rescaling
  RECT rect; AEVec2 viewport;
  AEGfxGetViewRectangle(&viewport.x, &viewport.y);
  m_ratio = viewport.x / viewport.y;
  GetClientRect(AEGetWindowHandler(), &rect);
  AEVec2 client = AEVec2(rect.right - rect.left, rect.bottom - rect.top);
  if (viewport != client) AEGfxSetViewRectangle(client.x, client.y);
}

void Camera::UpdateMatrix() {
  // View Space
  auto rotation = AEMtx44::Rotate(0, 0, transform.rotation);
  auto translation = AEMtx44::Translate(-transform.position.x, -transform.position.y, 0);
  auto view = rotation * translation;

  // Clip Space
  AEMtx44 clip = AEMtx44::OrthoProjGL(1/m_size, 1/m_size, m_near, m_far);
  m_cameraMatrix = clip.MultThis(view);
}

}
