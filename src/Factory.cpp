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
}

AEGfxTexture* Factory::LoadTexture(const char* filepath) {
  try {
    std::cout << "[Factory] Texture " << filepath << " already exists\n";
    return m_textures.at(filepath);
  } catch (const std::out_of_range &e) {
    std::cout << "[Factory] Texture " << filepath << " does not exist yet. Adding to pool...\n";
    m_textures.emplace(filepath, AEGfxTextureLoad(filepath));
    return m_textures.at(filepath);
  }
}

void Factory::FreeTexture(const char *filepath) {
  try {
    AEGfxTextureUnload(m_textures.at(filepath));
    m_textures.erase(filepath);
  } catch (const std::out_of_range &e) {
    std::cout << "[Factory] The texture you are trying to delete doesn't exist";
  }
}
void Factory::FreeAllTextures() {
  for (auto& [filepath, texture]: m_textures) {
    AEGfxTextureUnload(texture);
    m_textures.erase(filepath);
  }
}

} // namespace FNFE
