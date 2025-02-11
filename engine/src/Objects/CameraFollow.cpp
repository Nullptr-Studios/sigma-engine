#include "CameraFollow.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

#include "Actor.hpp"
namespace Sigma {

void CameraFollow::Start() {
  Camera::Start();
  if (!m_target) {
    std::cerr << "No Target set for the CameraFollow before start";
  }
}
void CameraFollow::Update(double deltaTime) {
  Camera::Update(deltaTime);
  LerpToPosition(m_target->transform.position,deltaTime);
}
void CameraFollow::LerpToPosition(glm::vec3 position, float delta) {
  glm::vec2 viewport;
  AEGfxGetViewRectangle(&viewport.x, &viewport.y);
  glm::vec2 screenPos = WorldToScreen({position.x,position.y});
  screenPos = {screenPos.x/viewport.x*2,screenPos.y/viewport.y*2};// What region of the screen the object is at 1,1 being top right -1,-1 bottom left
  // std::ostringstream oss; // DEBUG
  // oss << screenPos.x << " " << screenPos.y << "\n";
  if (m_safeZone.x < screenPos.x && screenPos.x < m_safeZone.y && m_safeZone.z < screenPos.y && screenPos.y < m_safeZone.w) {
    transform.position = glm::lerp(transform.position, position, delta);
    // oss << "a";
  } else if (m_warningZone.x < screenPos.x && screenPos.x < m_warningZone.y && m_warningZone.z < screenPos.y && screenPos.y < m_warningZone.w) {
    transform.position = glm::lerp(transform.position, position, 3.0f*delta);
    // oss << "b";
  } else {
    transform.position = glm::lerp(transform.position, position, 10.0f * delta);
    // oss << "c";
  }
  // AEGfxPrint(10, 240, 0xFFFFFFFF, oss.str().c_str());
}
} // Sigma
#undef GLM_USE_EXPERIMENTAL
