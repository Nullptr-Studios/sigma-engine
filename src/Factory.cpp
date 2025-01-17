#include "Factory.hpp"

namespace FNFE {

Factory *Factory::m_instance = nullptr;

Factory::~Factory() {
  DestroyAllObjects();
  FreeAllTextures();

  m_instance = nullptr;
}

void Factory::DestroyObject(id_t id) {
  m_objects[id]->Destroy();
  m_objects.erase(id);
}

void Factory::DestroyAllObjects() {
  for (auto &[id, obj]: m_objects) {
    obj->Destroy();
    obj.reset();
  }
  m_objects.clear();
  m_renderables.clear();
}

AEGfxTexture* Factory::LoadTexture(const char* filepath) {
  if (filepath == nullptr) return nullptr;

  if (m_textures.contains(filepath)) {
    std::cout << "[Factory] Texture " << filepath << " already exists\n";
    return m_textures[filepath];
  } else {
    std::cout << "[Factory] Texture " << filepath << " does not exist yet. Adding to pool...\n";
    auto t = m_textures.emplace(filepath, AEGfxTextureLoad(filepath));
    return t.first->second;
  }
}

void Factory::FreeTexture(const char *filepath) {
  try {
    AEGfxTextureUnload(m_textures.at(filepath));
    m_textures.erase(filepath);
  } catch (const std::out_of_range &e) {
    std::cout << "[Factory] The texture \"" << filepath << "\" doesn't exist. Please create it before trying to delete it\n";
  }
}
void Factory::FreeAllTextures() {
  for (auto& [filepath, texture]: m_textures) {
    AEGfxTextureUnload(texture);
    m_textures.erase(filepath);
  }
}

}
