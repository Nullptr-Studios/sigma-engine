#include "UIText.hpp"
#include "aecore/AEMtx33.h"

namespace FNFE {

  void UIText::Init(const char * txt,unsigned color) {
    Object::Init();
  	m_text = txt;
    m_fontSize = 60;
    m_imgSize = 3;
  	m_color = color;
    m_font = AEGfxFontCreate("res/fonts/arial.ttf", m_fontSize, m_imgSize);
  }

  void UIText::Update(double deltaTime) {
	  AEMtx33 scale;
	  AEMtx33 rotate;
	  AEMtx33 translate;
	  AEMtx33 world;
	  AEMtx33Scale(&scale, transform.scale.x,transform.scale.y);
	  AEMtx33Rot(&rotate, transform.rotation);
	  AEMtx33Trans(&translate, transform.position.x,transform.position.y);
	  world = translate * rotate * scale;
	  AEGfxSetTransform(&world);
  	AEGfxFontDraw(m_text.c_str(),m_font,m_color);
  	
  }


  void UIText::Destroy() {
    Object::Destroy();
  	AEGfxFontFree(m_font);
  }
}
