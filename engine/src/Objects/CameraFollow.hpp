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
class Object;
/**
 * @class CameraFollow a camera that follows a target
 */

//TODO: Camera needs to support 2 players via scaling and more stuff
class CameraFollow : public Camera {
public:
  explicit CameraFollow(id_t id) : Camera(id) {}
  void Start() override;
  void Update(double deltaTime) override;

  float lerpScale = 1.0f; ///<@brief scale of lerp

  float baseDistance = 300.0f; ///<@brief base distance between players

  // TODO: WHO OVERWROTE THIS -x
  // no time to fix it now
  float size = 3.0f; ///< @brief Size of the camera
  
  Object * m_targetP1{}; ///<@brief target to follow
  Object * m_targetP2{}; ///<@brief target to follow
  glm::vec4 m_safeZone{}; ///@brief centermost zone (-x,x,-y,y)
  glm::vec4 m_warningZone{}; ///@brief middle zone (-x,x,-y,y)
private:
  /**
   * @brief will lerp camera toward target position
   * will use diffrent levels of lerp depending on region
   * of screen target is curently at
   * @param target posistion
   * @param delta percentage toward target
   */
  void LerpToPosition(glm::vec3 target, float delta);

  void LerpScale(float scale, float delta);
};

} // Sigma
