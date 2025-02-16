#include "UIProgressBar.hpp"

void Sigma::UIProgressBar::Start() {
  UIImage::Init();
  m_scale = transform.scale;
}

void Sigma::UIProgressBar::Update(double delta) {
  UIImage::Update(delta);
  if (m_progress != -1 && m_progress <= 0) {
    m_progress = -1;
    transform.scale = {0,0};
    OnEnd();
    return;
  }
  transform.scale.x = m_progress*m_scale.x;
}
