#include "CameraController.hpp"
#include "Random.hpp"
#include "Objects/Camera.hpp"
#include "aecore/imgui/imgui.h"
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

void CameraController::Update(double deltaTime) {
  Object::Update(deltaTime);
  ScreenShake(deltaTime);
  ImGui::Begin("ScreenShake");
  if (ImGui::Button("screenshake")) {
    StartShake(10, 1000, CONSTANT);
  }
  ImGui::End();
}

void CameraController::StartShake(float length, float magnitude, CameraShakeType type) {
  m_isScreenShake = true;
  m_ScreenShakeLength = length;
  m_ScreenShakeTimer = 0;
  m_ScreenShakeMag = magnitude;
  m_ScreenShakeType = type;
}

void CameraController::ScreenShake(float delta) {
  if (!m_isScreenShake) { return; }
  if (m_ScreenShakeTimer >= m_ScreenShakeLength) {
    m_isScreenShake = false;
    m_ScreenShakeLength = 0;
    m_ScreenShakeTimer = 0;
    m_ScreenShakeMag = 0;
  }
  m_ScreenShakeTimer += delta;
  float magnitude = 0;
  switch (m_ScreenShakeType) {
    case CONSTANT:
      magnitude = m_ScreenShakeMag;
    break;
    case EASE_IN:
      magnitude = m_ScreenShakeMag * (m_ScreenShakeTimer / m_ScreenShakeLength);
    break;
    case EASE_OUT:
      magnitude = m_ScreenShakeMag * (1 - (m_ScreenShakeTimer / m_ScreenShakeLength));
    break;
  }
  if (true) {
    glm::vec2 rand = Random::Circle(magnitude * delta);
    m_currentCamera->transform.position.x += rand.x;
    m_currentCamera->transform.position.y += rand.y;
  }

}

} // namespace Sigma
