/**
 * @file CameraFollow.hpp
 * @author dante
 * @date 2/8/2025
 *
 * @brief Camera that will follow an actor based on regions
 * of the camera and will apply diffrent rates/methods based on region of camera
 * object is based in
 */
#pragma once
#include "Camera.hpp"
namespace Sigma {
class Actor;
/**
 * @class CameraFollow a camera that follows a target
 */
class CameraFollow : public Camera {
public:
  explicit CameraFollow(id_t id) : Camera(id) {}
  void Start() override;
  void Update(double deltaTime) override;
  
  
  Actor * m_target; ///<@brief target to follow
  glm::vec4 m_safeZone; ///@brief centermost zone (-x,x,-y,y)
  glm::vec4 m_warningZone; ///@brief middle zone (-x,x,-y,y)
private:
  /**
   * @brief will lerp camera toward target position
   * will use diffrent levels of lerp depending on region
   * of screen target is curently at
   * @param target posistion
   * @param delta percentage toward target
   */
  void LerpToPosition(glm::vec3 target, float delta);
};

} // Sigma
