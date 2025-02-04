
#include "AnimationSystem.hpp"

#include <Core.hpp>
#include <Factory.hpp>


#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/matrix_transform_2d.hpp"

Sigma::ANIMATION::AnimationSystem* Sigma::ANIMATION::AnimationSystem::m_AnimSysinstance = nullptr;

// TODO: cleanup
// TODO: Add callback string implementation
// TODO: Support for trimmed sprites
Sigma::ANIMATION::TextureAtlas* Sigma::ANIMATION::AnimationSystem::LoadTextureAtlas(const char *jsonFilePath)
{
  //Profiler time
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
  a.frameRate = 12; // for now hard coded
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
    }else {
      
      for (auto & m_animation : m_animations) {
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

Sigma::ANIMATION::TextureAtlas *Sigma::ANIMATION::AnimationSystem::GetTextureAtlas(const char *name) {
  // check if TextureAtlas is already loaded
  if (m_loadedTextureAtlases.contains(name)) {
    // std::cout << "[AnimationSystem] Texture Atlas already loaded\n";
    return &m_loadedTextureAtlases[name];
  }

  std::cout << "[AnimationSystem] Texture Atlas not found\n";
  return nullptr;
}
void Sigma::ANIMATION::AnimationSystem::BuildTextureTransform(glm::mat3 &texMtx, const glm::vec2 framePosition,
                                                             const glm::vec2 frameSize, const glm::vec2 atlasSize) {
  glm::vec2 UVs = framePosition / atlasSize;

  float correctedY = atlasSize.y - (framePosition.y + frameSize.y);
  glm::vec2 translate = glm::vec2(framePosition.x/atlasSize.x, correctedY/atlasSize.y);

  texMtx = glm::mat3(1.0f);
  glm::scale(texMtx, UVs);
  glm::translate(texMtx, translate);
}

void Sigma::ANIMATION::AnimationSystem::BuildTextureTransform(glm::mat3& texMtx, const Frame* frame, const TextureAtlas* atlas)
{
  BuildTextureTransform(texMtx, frame->position, frame->size, atlas->size);
}

