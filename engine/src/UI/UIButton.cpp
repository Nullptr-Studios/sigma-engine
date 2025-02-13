#include "UIButton.hpp"
#include "Collision/Collision.hpp"
#include "Controller/CameraController.hpp"
#include "GlmAlphaTools.hpp"
#include "Objects/Camera.hpp"

namespace Sigma {

void UIButton::Init() { UIImage::Init(); }

void UIButton::Update(double delta) {
  UIImage::Update(delta);
  auto mousePosition = AEGetMouseData().position;
  glm::vec mousePos = glm::FromAEX(mousePosition);
  mousePos = GET_CAMERA->GetCurrentCamera()->ScreenToWorld(mousePos);
  bool pressed = AEInputMousePressed(AE_MOUSE_LEFT);
  bool hovered = ((transform.position.x - (transform.scale.x/2) <= mousePos.x && mousePos.x <= transform.position.x + (transform.scale.x/2)) &&
                  (transform.position.y - (transform.scale.y/2) <= mousePos.y && mousePos.y <= transform.position.y + (transform.scale.y/2)));
  if (!m_pressed && pressed && hovered) {
    OnClick();
  } else if (m_pressed && pressed && hovered) {
    OnHoldClick();
  } else if (m_pressed && (!pressed && !hovered)) {
    OnUnClick();
  }
  if (!m_hovered && hovered) {
    OnHoverStart();
  } else if (m_hovered && hovered) {
    OnHoverHold();
  } else if (m_hovered && !hovered) {
    OnHoverExit();
  }
  m_pressed = pressed;
  m_hovered = hovered;
}

} // namespace Sigma
