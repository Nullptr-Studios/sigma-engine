#include "CameraFollow.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

#include "Actor.hpp"
namespace Sigma {

void CameraFollow::Start() {
  Camera::Start();
  if (!m_targetP1) {
    std::cerr << "No Target set for the CameraFollow before start";
  }
}
void CameraFollow::Update(double deltaTime) {
  Camera::Update(deltaTime);

  //calculate midpoint between 2 players (if coop)
  if (m_targetP2 != nullptr) {
    glm::vec3 p = {(m_targetP1->transform.position.x + m_targetP2->transform.position.x) / 2, (m_targetP1->transform.position.y + m_targetP2->transform.position.y) / 2, 0};
    LerpToPosition(p,deltaTime);
    float dist = glm::distance(m_targetP1->transform.position, m_targetP2->transform.position);
    float s = size - (dist / baseDistance)*.25f;
    LerpScale(s,deltaTime);
    //std::cout << s << "\n";
    
  }else if (m_targetP1 != nullptr) {
    glm::vec3 p = {m_targetP1->transform.position.x, m_targetP1->transform.position.y, 0};
    LerpToPosition(p,deltaTime);
    LerpScale(size,deltaTime);
  }
}
void CameraFollow::LerpToPosition(glm::vec3 position, float delta) {
  glm::vec2 viewport;
  AEGfxGetViewRectangle(&viewport.x, &viewport.y);
  glm::vec2 screenPos = WorldToScreen({position.x, position.y});
  screenPos = {screenPos.x / viewport.x * 2,
               screenPos.y / viewport.y *
                   2}; // What region of the screen the object is at 1,1 being top right -1,-1 bottom left
  // std::ostringstream oss; // DEBUG
  // oss << screenPos.x << " " << screenPos.y << "\n";
  if (m_safeZone.x < screenPos.x && screenPos.x < m_safeZone.y && m_safeZone.z < screenPos.y &&
      screenPos.y < m_safeZone.w) {
    transform.position = glm::lerp(transform.position, position, delta);
    // oss << "a";
  } else if (m_warningZone.x < screenPos.x && screenPos.x < m_warningZone.y && m_warningZone.z < screenPos.y &&
             screenPos.y < m_warningZone.w) {
    transform.position = glm::lerp(transform.position, position, lerpScale * delta);
    // oss << "b";
  } else {
    transform.position = glm::lerp(transform.position, position, lerpScale * 2 * delta);
    // oss << "c";
  }
  // AEGfxPrint(10, 240, 0xFFFFFFFF, oss.str().c_str());
}
void CameraFollow::LerpScale(float scale, float delta) {
  Camera::size = scale;
}


} // Sigma
#undef GLM_USE_EXPERIMENTAL
