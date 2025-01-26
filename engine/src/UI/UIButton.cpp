#include "UIButton.hpp"
#include "Collision/Collider.hpp"
#include "Collision/Collision.hpp"

namespace FNFE {
  void UIButton::Init() {
    UIImage::Init();
    auto collider = new Collision::RectCollider;
    collider->flag = UI;
    collider->m_boxPoints.push_back({0,0,0});
    collider->m_boxScales.push_back({transform.scale.x,transform.scale.y,0});
    SetCollider(collider);
  }

  void UIButton::Update(double delta) {
    UIImage::Update(delta);
    auto collider = GetCollider();
    auto data = AEGetMouseData();
    AEVec3 mousePos = data.position.ToVec3();
    AEVec3 mouseScale = {0,0,0};
    
    bool pressed = AEInputMousePressed(AE_MOUSE_LEFT);
    bool hovered = Collision::RectOnRect(collider->m_boxPoints[0], collider->m_boxScales[0], mousePos, mouseScale);
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
}
