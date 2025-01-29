#include "UIText.hpp"
#include "GlmAlphaTools.hpp"
#include "aecore/AEGraphics.h"

namespace FNFE {

void UIText::Init(const char *txt, unsigned color) {
  Object::Init();
  m_text = txt;
  m_fontSize = 60;
  m_imgSize = 3;
  m_color = color;
  m_font = AEGfxFontCreate("res/fonts/arial.ttf", m_fontSize, m_imgSize);
}

void UIText::Update(double deltaTime) {
  auto matrix = glm::mat3(1.0f);
  matrix = glm::scale(matrix, transform.scale);
  matrix = glm::rotate(matrix, transform.rotation);
  matrix = glm::translate(matrix, glm::vec2(transform.position.x, transform.position.y));

  auto mtxAEX = glm::ToAEX(matrix);
  AEGfxSetTransform(&mtxAEX);
  AEGfxFontDraw(m_text.c_str(), m_font, m_color);
}

void UIText::Destroy() {
  Object::Destroy();
  AEGfxFontFree(m_font);
}

} // namespace FNFE
