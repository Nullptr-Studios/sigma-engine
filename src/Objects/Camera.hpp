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

  AEMtx44 CameraMatrix = AEMtx44::Identity();
  AEMtx44 GetCameraMatrix() {
    return CameraMatrix;
  }
  // WTF windows.h macroing "near" and "far" as if they weren't common english words -x
  void SetRenderDistance(const float near_, const float far_) { m_near = near_; m_far = far_; }
  void UpdateMatrix();

private:
  Transform m_oldTransform;

  float m_near =    0.0f;
  float m_far  = 1000.0f;
};

}
