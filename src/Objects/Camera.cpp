/**
 * @file Camera.cpp
 * @author Dario
 * @date 11/01/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#include "Camera.hpp"

namespace FNFE {

void Camera::Start() {
  Object::Start();

  m_oldTransform = transform;
  UpdateMatrix();
}

void Camera::Update(double deltaTime) {
  Object::Update(deltaTime);

  if (m_oldTransform.position != transform.position || m_oldTransform.rotation != transform.rotation) {
    UpdateMatrix();
    m_oldTransform = transform;
  }
}

void Camera::UpdateMatrix() {
  AEVec2 viewport;
  AEGfxGetViewRectangle(&viewport.x, &viewport.y);
  CameraMatrix = AEMtx44::OrthoProjGL(viewport.x, viewport.y, m_near, m_far);
  CameraMatrix.RotateThis(0.0f, 0.0f, transform.rotation);
  CameraMatrix.TranslateThis(-transform.position.x, -transform.position.y, 0);
}

}
