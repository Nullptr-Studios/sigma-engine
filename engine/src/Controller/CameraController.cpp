#include "CameraController.hpp"

#include "Objects/Camera.hpp"
namespace Sigma {
CameraController *CameraController::m_instance = nullptr;

CameraController *CameraController::GetCameraControllerInstance() {
  if (!m_instance) {
    std::cerr << "Camera controller has been called but it has not been created yet\n";
    return nullptr;
  }
  return m_instance;
}
Camera *CameraController::GetCurrentCamera() {
  if (!m_currentCamera) {
    std::cerr << "No Camera Currently in Use\n";
    return nullptr;
  }
  return m_currentCamera;
}
void CameraController::SetCurrentCamera(Camera *camera) {
  // TODO: events
  m_currentCamera = camera;
}



void CameraController::LerpCamera(glm::vec3 position, float delta) {
  // TODO:
  auto camPos = m_currentCamera->transform.position;
  m_currentCamera->transform.position = camPos + (position - camPos) * delta;
}
} // namespace Sigma
