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
  // void Update(double deltaTime) override; latr idk

private:
  static CameraController *m_instance;
  Camera *m_currentCamera{};
};

} // namespace Sigma
