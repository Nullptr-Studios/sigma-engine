#include "CameraController.hpp"

#include "Objects/Camera.hpp"
namespace FNFE {
CameraController *CameraController::m_instance = nullptr;

void CameraController::SetCurrentCamera(Camera *camera) {
  // TODO: events
  m_currentCamera = camera;
}
void CameraController::LerpCamera(glm::vec3 position, float delta) {
  //TODO:
  auto camPos = m_currentCamera->transform.position;
  m_currentCamera->transform.position = camPos + (position - camPos) * delta;
}
} // namespace FNFE
