#include "Actor.hpp"
#include "Factory.hpp"

namespace FNFE {

void Actor::Init() {
  Object::Init();

  AEGfxTriStart();
  AEGfxTriAdd(-0.5f, -0.5f, AE_COLORS_WHITE, 0.0f, 0.0f, -0.5f, 0.5f, AE_COLORS_WHITE, 0.0f, 1.0f, 0.5f, -0.5f,
              AE_COLORS_WHITE, 1.0f, 0.0f);
  AEGfxTriAdd(-0.5f, 0.5f, AE_COLORS_WHITE, 0.0f, 1.0f, 0.5f, 0.5f, AE_COLORS_WHITE, 1.0f, 1.0f, 0.5f, -0.5f,
              AE_COLORS_WHITE, 1.0f, 0.0f);

  m_triangleList = AEGfxTriEnd();

  m_texture = FNFE_FACTORY->LoadTexture(m_texturePath);
}

void Actor::Destroy() {
  Object::Destroy();
  AEGfxTextureUnload(m_texture);
  m_texture = nullptr;
  AEGfxTriFree(m_triangleList);
}

void Actor::SetTexture(const char* path) {
  m_texturePath = path;
  m_texture = FNFE_FACTORY->LoadTexture(m_texturePath);
}

}
