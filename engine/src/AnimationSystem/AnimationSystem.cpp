
#include "AnimationSystem.hpp"

#include <Core.hpp>
#include <Factory.hpp>
#include <fstream>

FNFE::ANIMATION::AnimationSystem* FNFE::ANIMATION::AnimationSystem::m_AnimSysinstance = nullptr;

// TODO: support for multiple animations
// TODO: cleanup
FNFE::ANIMATION::TextureAtlas* FNFE::ANIMATION::AnimationSystem::LoadTextureAtlas(const char *jsonFilePath)
{
  //Profiler time
  PROFILER_START;
  
  std::fstream file(jsonFilePath);
  if (!file.is_open()) {
    std::cout << "[AnimationSystem] failed to open JSON file " << jsonFilePath << '\n';
    return nullptr;
  }
  
  std::cout << "[AnimationSystem] Loading JSON file: " << jsonFilePath << '\n';
  json_t J = json_t::parse(file);

  // check if TextureAtlas is already loaded
  if (m_loadedTextureAtlases.contains(J["meta"]["image"])) {
    std::cout << "[AnimationSystem] Texture Atlas already loaded\n";
    return &m_loadedTextureAtlases[J["meta"]["image"]];
  }

  Animation a;
  a.frameRate = 12; // for now hard coded
  a.animationName = "Default";

  std::vector<Frame> m_frames;
  std::vector<Animation> m_animations;
  m_animations.emplace_back(a);
  
  // first save all the frames
  // Then sort frames into animations
  
  for (auto frame: J["frames"]) {
    Frame f;

    f.frameName = frame["filename"];
    f.frameSize = {frame["frame"]["w"], frame["frame"]["h"]};
    f.framePosition = {frame["frame"]["x"], frame["frame"]["y"]};
    // f.rotated = frame["rotated"];
    f.spriteSourceSize = {frame["spriteSourceSize"]["w"], frame["spriteSourceSize"]["h"]};
    f.spriteSourcePosition = {frame["spriteSourceSize"]["x"], frame["spriteSourceSize"]["y"]};
    // f.pivot = {frame["pivot"]["x"], frame["pivot"]["y"]};
    // f.trimmed = frame["trimmed"];
    // m_frames.emplace_back(f);

    auto index = f.frameName.find_last_of('_');
    std::string animName = f.frameName.substr(0, index);
    bool found = false;

    // TODO FIX SHITTY CODE
    if (animName.contains(".png")) {
      // default animation
      m_animations[0].frames.emplace_back(f);
      found = true;
    }else {
      
      for (auto & m_animation : m_animations) {
        if (m_animation.animationName == animName) {
          m_animation.frames.emplace_back(f);
          found = true;
          break;
        }
      }
      
      if (!found) {
        Animation a;
        a.frameRate = 12; // for now hard coded
        a.animationName = animName;
        a.frames.emplace_back(f);
        m_animations.emplace_back(a);
      }
    }
  }

  TextureAtlas ta;
  ta.filePath = J["meta"]["image"];
  ta.size = {J["meta"]["size"]["w"], J["meta"]["size"]["h"]};
  ta.animations = m_animations;

  ta.texture = FNFE_FACTORY->LoadTexture(ta.filePath.c_str());
  std::cout << "[AnimationSystem] Texture Atlas loaded\n";
  
  PROFILER_END("AnimationSystem::LoadTextureAtlas");
  
  return &m_loadedTextureAtlases.emplace(ta.filePath, ta).first->second;

}

FNFE::ANIMATION::TextureAtlas* FNFE::ANIMATION::AnimationSystem::GetTextureAtlas(const char* name)
{
  // check if TextureAtlas is already loaded
  if (m_loadedTextureAtlases.contains(name)) {
    // std::cout << "[AnimationSystem] Texture Atlas already loaded\n";
    return &m_loadedTextureAtlases[name];
  }

  std::cout << "[AnimationSystem] Texture Atlas not found\n";
  return nullptr;
}

// TODO: support for trimmed sprites
void FNFE::ANIMATION::AnimationSystem::BuildTextureTransform(AEMtx33 *pTexMtx, float sX, float sY, float sW, float sH, float taW, float taH)
{
  float correctedY = taH - (sY + sH);
  float vTranslateY = correctedY / taH;

  float correctedX = (sX);
  float uTranslateX = correctedX / taW;

  float uWidth = (sW / taW);
  float vHeight = (sH / taH);
  
  // Reset to identity
  AEMtx33Identity(pTexMtx);

  // Scale
  AEMtx33ScaleApply(pTexMtx, pTexMtx, uWidth, vHeight);

  // Translate
  AEMtx33TransApply(pTexMtx, pTexMtx, uTranslateX, vTranslateY);
}


void FNFE::ANIMATION::AnimationSystem::BuildTextureTransform(AEMtx33 *pTexMtx, Frame* frame, TextureAtlas* atlas)
{
  BuildTextureTransform(pTexMtx, frame->framePosition.x, frame->framePosition.y, frame->frameSize.x, frame->frameSize.y, atlas->size.x, atlas->size.y);
}

