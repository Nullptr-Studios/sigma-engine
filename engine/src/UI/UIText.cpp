#include "UIText.hpp"
#include "aecore/AEMtx33.h"

namespace FNFE {

  void UIText::Init() {
    Object::Init();
    m_fontSize = 48;
    m_imgSize = 20;
    m_font = AEGfxFontCreate("res/fonts/alarm clock.ttf", m_fontSize, m_imgSize);
  }

  void UIText::Update(double deltaTime) {
	  AEMtx33 scale;
	  AEMtx33 rotate;
	  AEMtx33 translate;
	  AEMtx33 world;
	  AEMtx33Scale(&scale, 1, 1);
	  AEMtx33Rot(&rotate, 0);
	  AEMtx33Trans(&translate, -300, 0);
	  world = translate * rotate * scale;
	  AEGfxSetTransform(&world);
  	AEGfxFontDraw("something",m_font,AE_COLORS_BLUE);
  	
  }


  void UIText::Destroy() {
    Object::Destroy();
  	AEGfxFontFree(m_font);
  }
}
