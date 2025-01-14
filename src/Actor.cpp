#include "Actor.hpp"

namespace FNFE {

void Actor::Init() {
  Object::Init();

  AEGfxTriStart();
  AEGfxTriAdd(-0.5f, -0.5f, AE_COLORS_WHITE, 0.0f, 0.0f, -0.5f, 0.5f, AE_COLORS_WHITE, 0.0f, 1.0f, 0.5f, -0.5f,
              AE_COLORS_WHITE, 1.0f, 0.0f);
  AEGfxTriAdd(-0.5f, 0.5f, AE_COLORS_WHITE, 0.0f, 1.0f, 0.5f, 0.5f, AE_COLORS_WHITE, 1.0f, 1.0f, 0.5f, -0.5f,
              AE_COLORS_WHITE, 1.0f, 0.0f);

  m_triangleList = AEGfxTriEnd();

  ReloadTexture();
}

void Actor::Destroy() {
  Object::Destroy();
  AEGfxTextureUnload(m_texture);
  m_texture = nullptr;
  AEGfxTriFree(m_triangleList);
}

void Actor::ReloadTexture() {
  if (m_texture != nullptr) AEGfxTextureUnload(m_texture);
  m_texture = AEGfxTextureLoad("res/toast.png");
}

} // namespace FNFE
