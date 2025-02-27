#include "UIElement.hpp"

#include "Controller/CameraController.hpp"
#include "Objects/Camera.hpp"
namespace Sigma {
void UIElement::Start() {
  Actor::Start();
}
void UIElement::Update(double deltaTime) {
  Actor::Update(deltaTime);
  if (m_isScreenSpaceUI) {
    auto camera = GET_CAMERA->GetCurrentCamera();
    transform.position = (m_screenSpaceTransform.position / camera->size);
    transform.scale = m_screenSpaceTransform.scale / camera->size;
    transform.rotation = m_screenSpaceTransform.rotation;
  }
}
} // namespace Sigma
