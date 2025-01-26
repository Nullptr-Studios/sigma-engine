#include "UIProgressBar.hpp"

void FNFE::UIProgressBar::Init() {
  UIImage::Init();
  transform.scale = m_scale;
  //m_progress = 1;
}

void FNFE::UIProgressBar::Update(double delta) {
  UIImage::Update(delta);
  if (m_progress <= 0) {
    transform.scale = {0,0};
    return;
  }
  //m_progress -=delta *.01;
  transform.scale.x = m_progress*m_scale.x;
}
