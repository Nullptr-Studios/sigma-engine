/**
 * @file Camera.hpp
 * @author Xein
 * @date 17/01/2025
 *
 * @brief Camera that provides a viewport for the game
 */

#pragma once
#include "Objects/Object.hpp"

namespace Sigma {

/**
 * @class Camera
 * @brief Provides a viewport for the game
 *
 * The camera class inherits from @c Object and is in charge of calculating the matrices that converts world coordinates
 * into window coordinates. First, it calculates the View Matrix, then it calculates the Clip matrix and lastly it
 * calculates the window matrix. Matrix calculation is done by @code window * clip * view @endcode 
 */
class Camera : public Object {
friend class CameraController;
public:
  explicit Camera(id_t id) : Object(id) {}

  void Start() override;
  void Update(double deltaTime) override;

  std::array<glm::mat4, 2> GetCameraMatrix() { return {m_viewMatrix, m_clipMatrix}; } ///< @brief returns the final Camera Transformation
  // WTF windows.h macroing "near" and "far" as if they weren't common english words -x
  /**
   * @brief Changes the furthest and nearest objects that can be seen by the clip transformation
   * @param near_ Near clip view
   * @param far_ Far clip view
   */
  void SetRenderDistance(const float near_, const float far_) { m_near = near_; m_far = far_; }

private:
  void UpdateMatrix(); ///< @brief Update the camera matrix (internal use only)

  Transform m_oldTransform;
  glm::mat4 m_viewMatrix = glm::mat4(1.0f);
  glm::mat4 m_clipMatrix = glm::mat4(1.0f);

  float m_ratio;           ///< @brief Ratio between width and height
  float m_size =     1.0f; ///< @brief Scaling of the camera by screen size
  float m_near = -1000.0f; ///< @brief Near clip plane
  float m_far  =  1000.0f; ///< @brief Far clip plane
};

}
