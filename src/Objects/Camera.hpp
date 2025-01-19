/**
 * @file Camera.hpp
 * @author Xein
 * @date 17/01/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include "Objects/Object.hpp"

namespace FNFE {

class Camera : public Object {
public:
  explicit Camera(id_t id) : Object(id) {}

  void Start() override;
  void Update(double deltaTime) override;

  AEMtx44 GetCameraMatrix() { return m_cameraMatrix; }
  // WTF windows.h macroing "near" and "far" as if they weren't common english words -x
  void SetRenderDistance(const float near_, const float far_) { m_near = near_; m_far = far_; }

private:
  void UpdateMatrix();
  
  Transform m_oldTransform;
  AEMtx44 m_cameraMatrix = AEMtx44::Identity();

  float m_size = 2.0f;
  float m_ratio;
  float m_near =    0.0f;
  float m_far  = 10.0f;
};

}
