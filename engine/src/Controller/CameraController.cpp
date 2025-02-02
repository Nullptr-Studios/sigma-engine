#include "CameraController.hpp"
namespace FNFE {
CameraController *CameraController::m_instance = nullptr;

void CameraController::SetCurrentCamera(Camera * camera) {
  m_currentCamera = camera;
}
}
