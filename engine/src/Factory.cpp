#include "Factory.hpp"
#include <ranges>

namespace Sigma {

Factory *Factory::m_instance = nullptr;

Factory::~Factory() {
  DestroyAllObjects();
  FreeAllTextures();

  m_instance = nullptr;
}

void Factory::FlushDestroyQueue() {
  for (auto id: m_destroyQueue) {
    if (m_log)
      std::cout << "[Factory] Destroying object " << m_objects[id]->GetName() << " with ID: " << id << "\n";

    if (m_objects[id]) {
      m_objects[id]->Destroy();
      m_renderables.erase(std::ranges::remove(m_renderables, id).begin(), m_renderables.end());
      delete m_objects[id];

      m_objects.erase(id);
    }
  }
  m_destroyQueue.clear();
}
void Factory::DestroyObject(const id_t id) {

  if (m_objects.contains(id)) {
    m_destroyQueue.push_back(id);
    return;
  }

  if (m_log)
    std::cout << "[Factory] Object with ID: " << id << " does not exist\n";
}

void Factory::DestroyObject(const Object *object) {
  if (object)
    DestroyObject(object->GetId());
}

void Factory::DestroyAllObjects() {
  PROFILER_START;
  for (const auto &obj: m_objects | std::views::values) {
    // DestroyObject(id);
    if (obj)
      obj->Destroy();
    delete obj;
  }
  m_objects.clear();
  m_renderables.clear();
  PROFILER_END("Factory::DestroyAllObjects")
}

Object *Factory::GetObjectAt(const id_t id) { return m_objects[id]; }

Object *Factory::FindObject(const std::string &name) {
  for (auto &obj: m_objects | std::views::values) {
    if (obj->GetName() == name)
      return obj;
  }

  return nullptr;
}

AEGfxTexture *Factory::LoadTexture(const char *filepath) {
  PROFILER_START

  if (filepath == nullptr)
    return nullptr;

  // @dante fixed a bug here :) -x
  if (m_textures.contains(filepath)) {
    if (m_log)
      std::cout << "[Factory] Texture " << filepath << " already exists\n";
    return m_textures[filepath];
  }

  if (m_log)
    std::cout << "[Factory] Texture " << filepath << " does not exist yet. Adding to pool...\n";
  auto Tx = AEGfxTextureLoad(filepath);
  if (Tx == nullptr) {
    std::cout << "[Factory] Texture " << filepath << " failed to load\n";
    return nullptr;
  }
  AEGfxTextureSetFilters(Tx, AE_GFX_TF_NEAREST, AE_GFX_TF_NEAREST);
  auto [fst, snd] = m_textures.emplace(filepath, Tx);

  PROFILER_END("Factory::LoadTexture")
  return fst->second;
}

void Factory::FreeTexture(const char *filepath) {
  if (m_textures.contains(filepath)) {
    AEGfxTextureUnload(m_textures[filepath]);
    m_textures.erase(filepath);
    return;
  }

  if (m_log) {
    std::cout << "[Factory] Texture " << filepath << " does not have a texture\n";
    std::cout << "[Factory] Unloading avoided\n";
  }
}
void Factory::FreeAllTextures() {
  for (const auto &texture: m_textures | std::views::values) {
    AEGfxTextureUnload(texture);
    // m_textures.erase(filepath);
  }
  m_textures.clear();
}
AEGfxFont *Factory::LoadFont(const char *filepath, int size) {
  if (m_fonts.contains(filepath)) {
    if (m_log)
      std::cout << "[Factory] Font " << filepath << " already exists\n";
    return m_fonts[filepath];
  }

  auto f = AEGfxFontCreate(filepath, size, 2);
  if (f == nullptr) {
    std::cout << "[Factory] Font " << filepath << " failed to load\n";
    return nullptr;
  }

  // Set the texture filters to nearest
  AEGfxTextureSetFilters(f->mFontTex, AE_GFX_TF_NEAREST, AE_GFX_TF_NEAREST);

  m_fonts.emplace(filepath, f);
  std::cout << "[Factory] Font " << filepath << " loaded\n";
  return f;
}

void Factory::InitializeTriList() {
  AEGfxTriStart();
  AEGfxTriAdd(-0.5f, -0.5f, AE_COLORS_WHITE, 0.0f, 0.0f, -0.5f, 0.5f, AE_COLORS_WHITE, 0.0f, 1.0f, 0.5f, -0.5f,
              AE_COLORS_WHITE, 1.0f, 0.0f);
  AEGfxTriAdd(-0.5f, 0.5f, AE_COLORS_WHITE, 0.0f, 1.0f, 0.5f, 0.5f, AE_COLORS_WHITE, 1.0f, 1.0f, 0.5f, -0.5f,
              AE_COLORS_WHITE, 1.0f, 0.0f);

  m_tris = AEGfxTriEnd();
}

} // namespace Sigma
