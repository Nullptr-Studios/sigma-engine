#include "UIImage.hpp"

#include "Controller/CameraController.hpp"
#include "Objects/Camera.hpp"
namespace Sigma {
void UIImage::Start() {
  Actor::Start();
}
void UIImage::Update(double deltaTime) {
  Actor::Update(deltaTime);
  if (m_isScreenSpaceUI) {
    auto camera = GET_CAMERA->GetCurrentCamera();
    transform.position = camera->transform.position + (m_screenSpaceTransform.position / camera->size);
    transform.scale = m_screenSpaceTransform.scale / camera->size;
  }
}
} // namespace Sigma
