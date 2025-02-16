#include "UIProgressBar.hpp"

void Sigma::UIProgressBar::Start() {
  UIImage::Init();
  m_scale = transform.scale;
}

void Sigma::UIProgressBar::Update(double delta) {
  UIImage::Update(delta);
  if (m_progress <= 0) {
    transform.scale = {0,0};
    return;
  }
  transform.scale.x = m_progress*m_scale.x;
}
