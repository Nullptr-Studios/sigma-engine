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
  if (m_currentCamera) {
    m_currentCamera->SetActive(false);
  }
  camera->SetActive(true);
  m_currentCamera = camera;
}
void CameraController::Start() {
  Object::Start();
  if (!m_currentCamera) {
    std::cerr << "No Camera set to the camera controller before start\n";
  } 
}

} // namespace Sigma
