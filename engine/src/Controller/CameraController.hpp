/**
 * @file CameraController.hpp
 * @author Dante Harper
 * @date 30/01/25
 *
 * @brief script that controlls the camera
 * TODO: Lerp with enum EaseIn EaseOut EaseInOut
 */
#pragma once
#include "Objects/Object.hpp"

enum CameraShakeType {
  CONSTANT,
  EASE_IN,
  EASE_OUT
};

namespace Sigma {
class Camera;
class CameraController : public Object {
public:
  explicit CameraController(id_t id) : Object(id) { m_instance = this; };
  CameraController(CameraController &&) = delete;
  CameraController(const CameraController &) = delete;

  static CameraController *GetCameraControllerInstance();
  Camera *GetCurrentCamera();
  void SetCurrentCamera(Camera *camera);

  
  // void Init() override; latr idk
  void Start() override;
  void Update(double deltaTime) override;
  
  void StartShake(float length, float magnitude, CameraShakeType type);

private:
  static CameraController *m_instance;
  Camera *m_currentCamera{};
  bool m_isScreenShake = false;
  float m_ScreenShakeMag{};
  float m_ScreenShakeTimer{};
  float m_ScreenShakeLength{};
  CameraShakeType m_ScreenShakeType{};

  void ScreenShake(float delta);
};

} // namespace Sigma
