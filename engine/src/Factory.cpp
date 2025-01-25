#include "Factory.hpp"

namespace FNFE {

Factory *Factory::m_instance = nullptr;

Factory::~Factory() {
  DestroyAllObjects();
  FreeAllTextures();

  m_instance = nullptr;
}

void Factory::DestroyObject(id_t id) {
  std::cout << "[Factory] Destroying object " << m_objects[id]->GetName() << " with ID: " << id << "\n";
  
  m_objects[id]->Destroy();
  m_objects[id].reset();
  m_renderables.erase(
    std::remove(m_renderables.begin(), m_renderables.end(), id), 
    m_renderables.end()
  );
  m_objects.erase(id);
}

void Factory::DestroyObejct(Object &object)
{
  DestroyObject(object.GetId());
}

void Factory::DestroyAllObjects() {
  for (auto &[id, obj]: m_objects) {
    obj->Destroy();
    obj.reset();
  }
  m_objects.clear();
  m_renderables.clear();
}

Object* Factory::GetObjectA(id_t id) {
  return m_objects[id].get();
}

AEGfxTexture* Factory::LoadTexture(const char* filepath) {
  if (filepath == nullptr) return nullptr;

  // @dante fixed a bug here :) -x
  if (m_textures.contains(filepath)) {
    std::cout << "[Factory] Texture " << filepath << " already exists\n";
    return m_textures[filepath];
  }
  
  std::cout << "[Factory] Texture " << filepath << " does not exist yet. Adding to pool...\n";
  auto Tx = AEGfxTextureLoad(filepath);
  AEGfxTextureSetFilters(Tx, AE_GFX_TF_NEAREST, AE_GFX_TF_NEAREST);
  auto t = m_textures.emplace(filepath, Tx);
  return t.first->second;
}

void Factory::FreeTexture(const char *filepath) {
  if (m_textures.contains(filepath)) {
    AEGfxTextureUnload(m_textures[filepath]);
    m_textures.erase(filepath);
    return;
  }

  std::cout << "[Factory] Texture " << filepath << " does not have a texture\n";
  std::cout << "[Factory] Unloading avoided\n";
}
void Factory::FreeAllTextures() {
  for (auto& [filepath, texture]: m_textures) {
    AEGfxTextureUnload(texture);
    // m_textures.erase(filepath);
  }
  m_textures.clear();
}

void Factory::InitializeTriList()
{
  AEGfxTriStart();
  AEGfxTriAdd(-0.5f, -0.5f, AE_COLORS_WHITE, 0.0f, 0.0f, -0.5f, 0.5f, AE_COLORS_WHITE, 0.0f, 1.0f, 0.5f, -0.5f,
              AE_COLORS_WHITE, 1.0f, 0.0f);
  AEGfxTriAdd(-0.5f, 0.5f, AE_COLORS_WHITE, 0.0f, 1.0f, 0.5f, 0.5f, AE_COLORS_WHITE, 1.0f, 1.0f, 0.5f, -0.5f,
              AE_COLORS_WHITE, 1.0f, 0.0f);

  m_tris = AEGfxTriEnd();
}

}
