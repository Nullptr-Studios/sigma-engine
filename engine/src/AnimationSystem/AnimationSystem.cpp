
#include "AnimationSystem.hpp"

#include <Core.hpp>
#include <Factory.hpp>


#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/matrix_transform_2d.hpp"

Sigma::Animation::AnimationSystem *Sigma::Animation::AnimationSystem::m_AnimSysinstance = nullptr;

// TODO: cleanup
// TODO: Support for trimmed sprites
// TODO: Support for pivot
Sigma::Animation::TextureAtlas *Sigma::Animation::AnimationSystem::LoadTextureAtlas(const char *jsonFilePath) {
  // Profiler time
  PROFILER_START;

  std::fstream file(jsonFilePath);
  if (!file.is_open()) {
    std::cout << "[AnimationSystem] failed to open JSON file " << jsonFilePath << '\n';
    file.close();
    return nullptr;
  }

  std::cout << "[AnimationSystem] Loading JSON file: " << jsonFilePath << '\n';
  json_t J = json_t::parse(file);

  // check if TextureAtlas is already loaded
  if (m_loadedTextureAtlases.contains(J["meta"]["image"])) {
    std::cout << "[AnimationSystem] Texture Atlas already loaded\n";
    file.close();
    return &m_loadedTextureAtlases[J["meta"]["image"]];
  }

  Animation a;
  a.frameRate = 30; // for now hard coded
  a.name = "Default";

  std::vector<Frame> m_frames;
  std::vector<Animation> m_animations;
  m_animations.emplace_back(a);

  // first save all the frames
  // Then sort frames into animations

  for (auto frame: J["frames"]) {
    Frame f;

    f.name = frame["filename"];
    f.size = {frame["frame"]["w"], frame["frame"]["h"]};
    f.position = {frame["frame"]["x"], frame["frame"]["y"]};
    // f.rotated = frame["rotated"];
    f.sourceSize = {frame["spriteSourceSize"]["w"], frame["spriteSourceSize"]["h"]};
    f.sourcePosition = {frame["spriteSourceSize"]["x"], frame["spriteSourceSize"]["y"]};

    if (frame.contains("pivot")) {
      f.pivot = {frame["pivot"]["x"], frame["pivot"]["y"]};
    }

    // f.pivot = {frame["pivot"]["x"], frame["pivot"]["y"]};
    // f.trimmed = frame["trimmed"];
    // m_frames.emplace_back(f);

    if (frame.contains("callback")) {
      f.AnimCallbackString = frame["callback"];
    }

    // Animation frame ownership
    auto index = f.name.find_last_of('_');
    std::string animName = f.name.substr(0, index);
    bool found = false;

    if (animName.contains(".png")) {
      // default animation
      m_animations[0].frames.emplace_back(f);
      found = true;
    } else {

      for (auto &m_animation: m_animations) {
        if (m_animation.name == animName) {
          m_animation.frames.emplace_back(f);
          found = true;
          break;
        }
      }

      if (!found) {
        Animation m_animation;
        m_animation.frameRate = 12; // for now hard coded
        m_animation.name = animName;
        m_animation.frames.emplace_back(f);
        m_animations.emplace_back(m_animation);
      }
    }
  }

  TextureAtlas ta;
  ta.filePath = J["meta"]["image"];
  ta.size = {J["meta"]["size"]["w"], J["meta"]["size"]["h"]};
  ta.animations = m_animations;

  ta.texture = GET_FACTORY->LoadTexture(ta.filePath.c_str());
  std::cout << "[AnimationSystem] Texture Atlas loaded\n";

  file.close();

  PROFILER_END("AnimationSystem::LoadTextureAtlas");

  return &m_loadedTextureAtlases.emplace(ta.filePath, ta).first->second;
}

Sigma::Animation::TextureAtlas *Sigma::Animation::AnimationSystem::GetTextureAtlas(const char *name) {
  // check if TextureAtlas is already loaded
  if (m_loadedTextureAtlases.contains(name)) {
    // std::cout << "[AnimationSystem] Texture Atlas already loaded\n";
    return &m_loadedTextureAtlases[name];
  }

  std::cout << "[AnimationSystem] Texture Atlas not found\n";
  return nullptr;
}
void Sigma::Animation::AnimationSystem::BuildTextureTransform(AEMtx33 *pTexMtx, float sX, float sY, float sW, float sH, float taW, float taH)
{
  float correctedY = taH - (sY + sH);
  float vTranslateY = correctedY / taH;

  float correctedX = (sX);
  float uTranslateX = correctedX / taW;
  
  float vHeight = (sH / taH);
  float uWidth = (sW / taW);
  
  // Reset to identity
  AEMtx33Identity(pTexMtx);

  // Scale
  AEMtx33ScaleApply(pTexMtx, pTexMtx, uWidth, vHeight);

  // Translate
  AEMtx33TransApply(pTexMtx, pTexMtx, uTranslateX, vTranslateY);
}

void Sigma::Animation::AnimationSystem::BuildTextureTransform(AEMtx33 *texMtx, const Frame *frame,
                                                              const TextureAtlas *atlas) {
  BuildTextureTransform(texMtx, frame->position.x, frame->position.y, frame->size.x, frame->size.y, atlas->size.x, atlas->size.y);
}
