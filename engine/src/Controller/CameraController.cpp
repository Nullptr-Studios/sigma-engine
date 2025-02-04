#include "CameraController.hpp"
namespace Sigma {
CameraController *CameraController::m_instance = nullptr;

void CameraController::SetCurrentCamera(Camera * camera) {
  m_currentCamera = camera;
}
}
