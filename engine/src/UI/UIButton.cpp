#include "UIButton.hpp"
#include "Collision/Collision.hpp"
#include "GlmAlphaTools.hpp"

namespace FNFE {

void UIButton::Init() { UIImage::Init(); }

void UIButton::Update(double delta) {
  UIImage::Update(delta);
  auto collider = GetCollider();
  auto mousePosition = AEGetMouseData().position.ToVec3();
  glm::vec3 mousePos = glm::FromAEX(mousePosition);
  glm::vec3 mouseScale = glm::vec3();

  bool pressed = AEInputMousePressed(AE_MOUSE_LEFT);
  glm::vec3 scale = {transform.scale.x, transform.scale.y, 0};
  bool hovered = Collision::RectOnRect(transform.position, scale, mousePos, mouseScale);
  if (!m_pressed && pressed && hovered) {
    OnPress();
  } else if (m_pressed && pressed && hovered) {
    OnHoldClick();
  } else if (m_pressed && (!pressed && !hovered)) {
    OnUnclick();
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

} // namespace FNFE
