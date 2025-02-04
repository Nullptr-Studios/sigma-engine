/**
 * @file CameraController.hpp
 * @author Dante Harper
 * @date 30/01/25
 *
 * @brief script that controlls the camera
 * TODO: Lerp with enum EaseIn EaseOut EaseInOut
 * TODO: Slerp maybe
 * TODO: folling player
 *      Split the camera into quadrents
 *      Center Area where camera does not move (safe)
 *      Bigger Zone where the camera eases into position untill player is in safe()
 *      Edges of the screen where the camera veclocity speeds up (warning)
 */
#pragma once
#include "Objects/Object.hpp"

namespace FNFE {
class Camera;
class CameraController : Object {
public:
  explicit CameraController(id_t id ) : Object(id) { m_instance = this; };
  CameraController(CameraController &&) = delete;
  CameraController(const CameraController &) = delete;

  static CameraController *GetCameraControllerInstance() {
    if (!m_instance) {
      return nullptr;
    }
    return m_instance;
  }
  Camera *GetCurrentCamera() { return m_currentCamera; }
  void SetCurrentCamera(Camera *camera);

  /**
   * @note does not lock z pos so be aware of that
   * @param position position of target location
   * @param delta percentange between [0,1]
   */
  void LerpCamera(glm::vec3 position, float delta);

private:
  static CameraController *m_instance;
  Camera *m_currentCamera{};
};

} // namespace FNFE
