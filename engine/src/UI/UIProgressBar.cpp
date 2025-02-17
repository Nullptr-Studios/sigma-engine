#include "UIProgressBar.hpp"

void Sigma::UIProgressBar::Start() {
  UIImage::Init();
  if (m_isScreenSpaceUI) {
    m_scale = m_screenSpaceTransform.scale;
  } else {
    m_scale = transform.scale;
  }
}

void Sigma::UIProgressBar::Update(double delta) {
  UIImage::Update(delta);
  if (m_progress != -1 && m_progress <= 0) {
    m_progress = -1;
    transform.scale = {0,1};
    OnEnd();
    return;
  }
  transform.scale.x = m_progress*m_scale.x;
  m_screenSpaceTransform.scale.x = m_progress*m_scale.x;
}
